#include "lighting.h"

#include "common/vk_common.h"
#include "gltf_loader.h"
#include "gui.h"
#include "platform/filesystem.h"
#include "platform/platform.h"
#include "scene_graph/components/orthographic_camera.h"
#include "stats/stats.h"
#include "common/vk_initializers.h"

LightingSample::LightingSample()
{
    auto &config = get_configuration();

	config.insert<clan::BoolSetting>(0, rotate_shadows, false);
	config.insert<clan::BoolSetting>(1, rotate_shadows, true);
}

void LightingSample::draw_gui()
{
	gui->show_options_window(
	    /* body = */ [this]() {
		    ImGui::Checkbox("Rotate shadows", &rotate_shadows);
	    },
	    /* lines = */ 1);
}

static VkExtent3D downsample_extent(const VkExtent3D &extent, uint32_t level)
{
	return {
	    std::max(1u, extent.width >> level),
	    std::max(1u, extent.height >> level),
	    std::max(1u, extent.depth >> level)};
}

void LightingSample::prepare_render_targets()
{
	// To make this sample demanding enough to saturate the tested mobile devices, use 4K.
	VkExtent3D size = {3840, 2160, 1};

	clan::Image color_targets{*device,
                              size,
                              VK_FORMAT_R16G16B16A16_SFLOAT,
	                          VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	                          VMA_MEMORY_USAGE_GPU_ONLY};

	clan::Image depth_targets{*device, size,
                              VK_FORMAT_D32_SFLOAT,
	                          VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	                          VMA_MEMORY_USAGE_GPU_ONLY};

    VkExtent3D              shadow_resolution{8 * 1024, 8 * 1024, 1};
    VkImageFormatProperties depth_properties{};
	vkGetPhysicalDeviceImageFormatProperties(device->get_gpu().get_handle(), VK_FORMAT_D16_UNORM, VK_IMAGE_TYPE_2D,
	                                         VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	                                         0, &depth_properties);

	shadow_resolution.width  = std::min(depth_properties.maxExtent.width, shadow_resolution.width);
	shadow_resolution.height = std::min(depth_properties.maxExtent.height, shadow_resolution.height);
	shadow_resolution.width  = std::min(device->get_gpu().get_properties().limits.maxFramebufferWidth, shadow_resolution.width);
	shadow_resolution.height = std::min(device->get_gpu().get_properties().limits.maxFramebufferHeight, shadow_resolution.height);

	clan::Image shadow_target{*device,
                              shadow_resolution,
                              VK_FORMAT_D16_UNORM,
	                          VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	                          VMA_MEMORY_USAGE_GPU_ONLY};

    for (uint32_t level = 1; level < 7; level++)
    {
		blur_chain.push_back(std::make_unique<clan::Image>(
		    *device, downsample_extent(size, level),
		    VK_FORMAT_R16G16B16A16_SFLOAT,
		    VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		    VMA_MEMORY_USAGE_GPU_ONLY));

		blur_chain_views.push_back(std::make_unique<clan::ImageView>(
		    *blur_chain.back(), VK_IMAGE_VIEW_TYPE_2D));
    }

	auto sampler_info         = clan::initializers::sampler_create_info();
	sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.minFilter    = VK_FILTER_LINEAR;
	sampler_info.magFilter    = VK_FILTER_LINEAR;
	sampler_info.maxLod       = VK_LOD_CLAMP_NONE;
    linear_sampler = std::make_unique<clan::Sampler>(*device, sampler_info);

    sampler_info.compareOp     = VK_COMPARE_OP_GREATER_OR_EQUAL;
	sampler_info.compareEnable = VK_TRUE;
	comparison_sampler         = std::make_unique<clan::Sampler>(*device, sampler_info);


	std::vector<clan::Image> color_attachments;
	color_attachments.push_back(std::move(color_targets));
	color_attachments.push_back(std::move(depth_targets));
	forward_render_targets = std::make_unique<clan::RenderTarget>(std::move(color_attachments));

	std::vector<clan::Image> shadow_attachments;
	shadow_attachments.push_back(std::move(shadow_target));
	shadow_render_target = std::make_unique<clan::RenderTarget>(std::move(shadow_attachments));
}

bool LightingSample::prepare(clan::Platform& platform)
{
	set_high_priority_graphics_queue_enable(true);

	if (!ClanSample::prepare(platform))
	{
		return false;
	}

    load_scene("scenes/bonza/Bonza.gltf");

	auto &camera_node = clan::add_free_camera(*scene, "main_camera", get_render_context().get_surface_extent());
	camera            = &camera_node.get_component<clan::sg::Camera>();

	// Attach a shadow camera to the directional light.
	auto lights = scene->get_components<clan::sg::Light>();
	for (auto &light : lights)
	{
		if (light->get_light_type() == clan::sg::LightType::Directional)
		{
			clan::sg::LightProperties props{};
			props.color = glm::vec3(50.0f, 40.0f, 30.0f);
			light->set_properties(props);
			auto *node = light->get_node();

			// Hardcoded to fit to the scene.
			auto ortho_camera = std::make_unique<clan::sg::OrthographicCamera>("shadow_camera",
			                                                                   -2000.0f, 3000.0f,
			                                                                   -2500.0f, 1500.0f,
			                                                                   -2000.0f, 2000.0f);

			ortho_camera->set_node(*node);
			scene->add_component(std::move(ortho_camera), *node);
			shadow_camera = &node->get_component<clan::sg::Camera>();
			break;
		}
	}

    prepare_render_targets();

	clan::ShaderSource vert_shader("lighting/forward.vert");
	clan::ShaderSource frag_shader("lighting/forward.frag");

	auto scene_subpass = std::make_unique<ShadowMapForwardSubpass>(get_render_context(),
                                                                   std::move(vert_shader),
                                                                   std::move(frag_shader),
                                                                   *scene, *camera,
                                                                   *shadow_camera);

	clan::ShaderSource shadow_vert_shader("lighting/shadow.vert");
	clan::ShaderSource shadow_frag_shader("lighting/shadow.frag");

	auto shadow_scene_subpass = std::make_unique<DepthMapSubpass>(get_render_context(),
                                                                  std::move(shadow_vert_shader),
                                                                  std::move(shadow_frag_shader),
                                                                  *scene, *shadow_camera);

    shadow_render_pipeline.add_subpass(std::move(shadow_scene_subpass));

	clan::ShaderSource composite_vert_shader("lighting/composite.vert");
	clan::ShaderSource composite_frag_shader("lighting/composite.frag");

    auto composite_scene_subpass = std::make_unique<CompositeSubpass>(get_render_context(),
                                                                      std::move(composite_vert_shader),
                                                                      std::move(composite_frag_shader));

    forward_render_pipeline.add_subpass(std::move(scene_subpass));

	forward_render_pipeline.set_load_store({{VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE},
	                                        {VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE}});

    clan::RenderPipeline blit_render_pipeline;
    blit_render_pipeline.add_subpass(std::move(composite_scene_subpass));

	blit_render_pipeline.set_load_store({{VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE},
	                                     {VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE}});

    set_render_pipeline(std::move(blit_render_pipeline));

    clan::CounterSamplingConfig config;

    config.mode = clan::CounterSamplingMode::Continuous;

	stats->request_stats({clan::StatIndex::frame_times,}, config);

    gui = std::make_unique<clan::Gui>(*this, platform.get_window(), stats.get());

	// Store the start time to calculate rotation
	start_time = std::chrono::system_clock::now();

	auto &threshold_module = device->get_resource_cache().request_shader_module(VK_SHADER_STAGE_COMPUTE_BIT,
	                                                                            clan::ShaderSource("lighting/threshold.comp"));
	auto &blur_up_module   = device->get_resource_cache().request_shader_module(VK_SHADER_STAGE_COMPUTE_BIT,
                                                                                clan::ShaderSource("lighting/blur_up.comp"));
	auto &blur_down_module = device->get_resource_cache().request_shader_module(VK_SHADER_STAGE_COMPUTE_BIT,
	                                                                            clan::ShaderSource("lighting/blur_down.comp"));

    threshold_pipeline     = &device->get_resource_cache().request_pipeline_layout({&threshold_module});
	blur_up_pipeline       = &device->get_resource_cache().request_pipeline_layout({&blur_up_module});
	blur_down_pipeline     = &device->get_resource_cache().request_pipeline_layout({&blur_down_module});

    present_graphics_queue = &device->get_queue_by_present(0);
	early_graphics_queue = present_graphics_queue;
	post_compute_queue   = present_graphics_queue;

    return true;
}

void LightingSample::render_shadow_pass()
{
	auto &queue          = *early_graphics_queue;
	auto &command_buffer = render_context->get_active_frame().request_command_buffer(queue);
	command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    auto &views = shadow_render_target->get_views();

	{
		clan::ImageMemoryBarrier memory_barrier{};
		memory_barrier.old_layout      = VK_IMAGE_LAYOUT_UNDEFINED;
		memory_barrier.new_layout      = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		memory_barrier.src_access_mask = 0;
		memory_barrier.dst_access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		memory_barrier.src_stage_mask  = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		memory_barrier.dst_stage_mask  = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

		command_buffer.image_memory_barrier(views.at(0), memory_barrier);
	}

	set_viewport_and_scissor(command_buffer, shadow_render_target->get_extent());
	shadow_render_pipeline.draw(command_buffer, *shadow_render_target, VK_SUBPASS_CONTENTS_INLINE);
	command_buffer.end_render_pass();

	{
		clan::ImageMemoryBarrier memory_barrier{};
		memory_barrier.old_layout      = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		memory_barrier.new_layout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		memory_barrier.src_access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		memory_barrier.src_stage_mask  = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		memory_barrier.dst_stage_mask  = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		command_buffer.image_memory_barrier(views.at(0), memory_barrier);
	}

    command_buffer.end();

    render_context->submit(queue, {&command_buffer});
}

VkSemaphore LightingSample::render_forward_offscreen_pass(VkSemaphore hdr_wait_semaphore)
{
	auto &queue          = *early_graphics_queue;
	auto &command_buffer = render_context->get_active_frame().request_command_buffer(queue);

    command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    auto &views = forward_render_targets->get_views();

	{
		clan::ImageMemoryBarrier memory_barrier{};
		memory_barrier.old_layout      = VK_IMAGE_LAYOUT_UNDEFINED;
		memory_barrier.new_layout      = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		memory_barrier.src_access_mask = 0;
		memory_barrier.dst_access_mask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		memory_barrier.src_stage_mask  = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		memory_barrier.dst_stage_mask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		command_buffer.image_memory_barrier(views.at(0), memory_barrier);
	}

	{
		clan::ImageMemoryBarrier memory_barrier{};
		memory_barrier.old_layout      = VK_IMAGE_LAYOUT_UNDEFINED;
		memory_barrier.new_layout      = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		memory_barrier.src_access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		memory_barrier.dst_access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		memory_barrier.src_stage_mask  = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		memory_barrier.dst_stage_mask  = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

		command_buffer.image_memory_barrier(views.at(1), memory_barrier);
	}

	set_viewport_and_scissor(command_buffer, forward_render_targets->get_extent());
	forward_render_pipeline.draw(command_buffer, *forward_render_targets, VK_SUBPASS_CONTENTS_INLINE);
	command_buffer.end_render_pass();

    {
		clan::ImageMemoryBarrier memory_barrier{};
		memory_barrier.old_layout      = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		memory_barrier.new_layout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		memory_barrier.src_access_mask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		memory_barrier.dst_access_mask = 0;
		memory_barrier.src_stage_mask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		memory_barrier.dst_stage_mask  = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		// In a release barrier, dst_stage_mask/access_mask should be BOTTOM_OF_PIPE/0.
		// We cannot access the resource anymore after all. Semaphore takes care of things from here.

		// Release barrier if we're going to read HDR texture in compute queue
		// of a different queue family index. We'll have to duplicate this barrier
		// on compute queue's end.
		if (early_graphics_queue->get_family_index() != post_compute_queue->get_family_index())
		{
			memory_barrier.old_queue_family = early_graphics_queue->get_family_index();
			memory_barrier.new_queue_family = post_compute_queue->get_family_index();
		}

		command_buffer.image_memory_barrier(views.at(0), memory_barrier);
	}

    command_buffer.end();

	// Conditionally waits on hdr_wait_semaphore.
	// This resolves the write-after-read hazard where previous frame tonemap read from HDR buffer.
	auto signal_semaphore = render_context->submit(queue, {&command_buffer},
	                                               hdr_wait_semaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

    if (hdr_wait_semaphore)
	{
		render_context->release_owned_semaphore(hdr_wait_semaphore);
	}

	return signal_semaphore;
}

VkSemaphore LightingSample::render_swapchain(VkSemaphore post_semaphore)
{
	auto &queue          = *present_graphics_queue;
	auto &command_buffer = render_context->get_active_frame().request_command_buffer(queue);

    command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	if (post_compute_queue->get_family_index() != present_graphics_queue->get_family_index())
	{
		// Purely ownership transfer here. No layout change required.
		clan::ImageMemoryBarrier memory_barrier{};
		memory_barrier.old_layout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		memory_barrier.new_layout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		memory_barrier.src_access_mask  = 0;
		memory_barrier.dst_access_mask  = 0;
		memory_barrier.src_stage_mask   = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		memory_barrier.dst_stage_mask   = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		memory_barrier.old_queue_family = post_compute_queue->get_family_index();
		memory_barrier.new_queue_family = present_graphics_queue->get_family_index();

		command_buffer.image_memory_barrier(forward_render_targets->get_views()[0], memory_barrier);
	}

    draw(command_buffer, render_context->get_active_frame().get_render_target());

    command_buffer.end();

	// We're going to wait on this semaphore in different frame,
	// so we need to hold ownership of the semaphore until we complete the wait.
	hdr_wait_semaphores = render_context->request_semaphore_with_ownership();

    // We've read the post buffer outputs, so we need to consider write-after-read
    compute_post_semaphore = render_context->request_semaphore_with_ownership();

	const VkSemaphore signal_semaphores[] = {
	    render_context->request_semaphore(),
	    hdr_wait_semaphores,
	    compute_post_semaphore,
	};

	const VkSemaphore wait_semaphores[] = {
	    post_semaphore,
	    render_context->consume_acquired_semaphore(),
	};

	const VkPipelineStageFlags wait_stages[] = {
	    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
	    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	};

	auto info                 = clan::initializers::submit_info();
	info.pSignalSemaphores    = signal_semaphores;
	info.signalSemaphoreCount = 3;
	info.pWaitSemaphores      = wait_semaphores;
	info.waitSemaphoreCount   = 2;
	info.pWaitDstStageMask    = wait_stages;
	info.commandBufferCount   = 1;
	info.pCommandBuffers      = &command_buffer.get_handle();

    queue.submit({info}, render_context->get_active_frame().request_fence());

    render_context->release_owned_semaphore(wait_semaphores[1]);

	return signal_semaphores[0];
}

VkSemaphore LightingSample::render_compute_post(VkSemaphore wait_graphics_semaphore, VkSemaphore wait_present_semaphore)
{
	auto &queue          = *post_compute_queue;
	auto &command_buffer = render_context->get_active_frame().request_command_buffer(queue);

	command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	// Acquire barrier if we're going to read HDR texture in compute queue
	// of a different queue family index. We'll have to duplicate this barrier
	// on compute queue's end.
	if (early_graphics_queue->get_family_index() != post_compute_queue->get_family_index())
	{
		clan::ImageMemoryBarrier memory_barrier{};
		memory_barrier.old_layout      = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		memory_barrier.new_layout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		memory_barrier.src_access_mask = 0;
		memory_barrier.dst_access_mask = VK_ACCESS_SHADER_READ_BIT;
		// Match pWaitDstStages for src stage here.
		memory_barrier.src_stage_mask   = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		memory_barrier.dst_stage_mask   = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		memory_barrier.old_queue_family = early_graphics_queue->get_family_index();
		memory_barrier.new_queue_family = post_compute_queue->get_family_index();

		command_buffer.image_memory_barrier(forward_render_targets->get_views()[0], memory_barrier);
	}

	const auto discard_blur_view = [&](const clan::ImageView &view) {
		clan::ImageMemoryBarrier memory_barrier{};

		memory_barrier.old_layout      = VK_IMAGE_LAYOUT_UNDEFINED;
		memory_barrier.new_layout      = VK_IMAGE_LAYOUT_GENERAL;
		memory_barrier.src_access_mask = 0;
		memory_barrier.dst_access_mask = VK_ACCESS_SHADER_WRITE_BIT;
		memory_barrier.src_stage_mask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		memory_barrier.dst_stage_mask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

		command_buffer.image_memory_barrier(view, memory_barrier);
	};

	const auto read_only_blur_view = [&](const clan::ImageView &view, bool final) {
		clan::ImageMemoryBarrier memory_barrier{};

		memory_barrier.old_layout      = VK_IMAGE_LAYOUT_GENERAL;
		memory_barrier.new_layout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		memory_barrier.src_access_mask = VK_ACCESS_SHADER_WRITE_BIT;
		memory_barrier.dst_access_mask = final ? 0 : VK_ACCESS_SHADER_READ_BIT;
		memory_barrier.src_stage_mask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		memory_barrier.dst_stage_mask  = final ? VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT : VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

		command_buffer.image_memory_barrier(view, memory_barrier);
	};

	struct Push
	{
		uint32_t width, height;
		float    inv_width, inv_height;
		float    inv_input_width, inv_input_height;
	};

	const auto dispatch_pass = [&](const clan::ImageView &dst, const clan::ImageView &src, bool final = false) {
		discard_blur_view(dst);

		auto dst_extent = downsample_extent(dst.get_image().get_extent(), dst.get_subresource_range().baseMipLevel);
		auto src_extent = downsample_extent(src.get_image().get_extent(), src.get_subresource_range().baseMipLevel);

		Push push{};
		push.width            = dst_extent.width;
		push.height           = dst_extent.height;
		push.inv_width        = 1.0f / float(push.width);
		push.inv_height       = 1.0f / float(push.height);
		push.inv_input_width  = 1.0f / float(src_extent.width);
		push.inv_input_height = 1.0f / float(src_extent.height);

		command_buffer.push_constants(push);
		command_buffer.bind_image(src, *linear_sampler, 0, 0, 0);
		command_buffer.bind_image(dst, 0, 1, 0);
		command_buffer.dispatch((push.width + 7) / 8, (push.height + 7) / 8, 1);

		read_only_blur_view(dst, final);
	};

	// A very basic and dumb HDR Bloom pipeline. Don't consider this a particularly good or efficient implementation.
	// It's here to represent a plausible compute post workload.
	// - Threshold pass
	// - Blur down
	// - Blur up
	command_buffer.bind_pipeline_layout(*threshold_pipeline);
	dispatch_pass(*blur_chain_views[0], forward_render_targets->get_views()[0]);

	command_buffer.bind_pipeline_layout(*blur_down_pipeline);
	for (uint32_t index = 1; index < blur_chain_views.size(); index++)
	{
		dispatch_pass(*blur_chain_views[index], *blur_chain_views[index - 1]);
	}

	command_buffer.bind_pipeline_layout(*blur_up_pipeline);
	for (uint32_t index = blur_chain_views.size() - 2; index >= 1; index--)
	{
		dispatch_pass(*blur_chain_views[index], *blur_chain_views[index + 1], index == 1);
	}

	// We're going to read the HDR texture again in the present queue.
	// Need to release ownership back to that queue.
	if (post_compute_queue->get_family_index() != present_graphics_queue->get_family_index())
	{
		clan::ImageMemoryBarrier memory_barrier{};
		memory_barrier.old_layout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		memory_barrier.new_layout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		memory_barrier.src_access_mask  = 0;
		memory_barrier.dst_access_mask  = 0;
		memory_barrier.src_stage_mask   = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		memory_barrier.dst_stage_mask   = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		memory_barrier.old_queue_family = post_compute_queue->get_family_index();
		memory_barrier.new_queue_family = present_graphics_queue->get_family_index();

		command_buffer.image_memory_barrier(forward_render_targets->get_views()[0], memory_barrier);
	}

	command_buffer.end();

	VkPipelineStageFlags wait_stages[]     = {VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT};
	VkSemaphore          wait_semaphores[] = {wait_graphics_semaphore, wait_present_semaphore};
	VkSemaphore          signal_semaphore  = render_context->request_semaphore();

	auto info                 = clan::initializers::submit_info();
	info.pSignalSemaphores    = &signal_semaphore;
	info.signalSemaphoreCount = 1;
	info.pWaitSemaphores      = wait_semaphores;
	info.waitSemaphoreCount   = wait_present_semaphore != VK_NULL_HANDLE ? 2 : 1;
	info.pWaitDstStageMask    = wait_stages;
	info.commandBufferCount   = 1;
	info.pCommandBuffers      = &command_buffer.get_handle();

	if (wait_present_semaphore != VK_NULL_HANDLE)
	{
		render_context->release_owned_semaphore(wait_present_semaphore);
	}

	queue.submit({info}, VK_NULL_HANDLE);
	return signal_semaphore;
}

void LightingSample::update(float delta_time)
{

	auto *forward_subpass   = static_cast<ShadowMapForwardSubpass *>(forward_render_pipeline.get_subpasses()[0].get());
	auto *composite_subpass = static_cast<CompositeSubpass *>(render_pipeline->get_subpasses()[0].get());

	forward_subpass->set_shadow_map(&shadow_render_target->get_views()[0], comparison_sampler.get());
	composite_subpass->set_texture(&forward_render_targets->get_views()[0], blur_chain_views[1].get(), linear_sampler.get());

	float rotation_factor = std::chrono::duration<float>(std::chrono::system_clock::now() - start_time).count();

	glm::quat orientation;

	// Lots of random jank to get a desired orientation quaternion for the directional light.
	if (rotate_shadows)
	{
		// Move shadows and directional light slightly.
		orientation = glm::normalize(
		    glm::angleAxis(glm::pi<float>(), glm::vec3(0.0f, -1.0f, 0.0f)) *
		    glm::angleAxis(-0.2f * glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) *
		    glm::angleAxis(glm::two_pi<float>() * glm::fract(rotation_factor * 0.05f), glm::vec3(0.0f, 0.0f, -1.0f)) *
		    glm::angleAxis(-0.05f * glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else
	{
		orientation = glm::normalize(
		    glm::angleAxis(glm::pi<float>(), glm::vec3(0.0f, -1.0f, 0.0f)) *
		    glm::angleAxis(-0.2f * glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));
	}

	auto &shadow_camera_transform = shadow_camera->get_node()->get_component<clan::sg::Transform>();
	shadow_camera_transform.set_rotation(orientation);

	// Explicit begin_frame and end_frame since we're doing async compute, many submissions and custom semaphores ...
	render_context->begin_frame();

    update_scene(delta_time);
	update_gui(delta_time);

	// Collect the performance data for the sample graphs
	update_stats(delta_time);

	// Setup render pipeline:
	// - Shadow pass
	// - HDR
	// - Async compute post
	// - Composite
	render_shadow_pass();
	VkSemaphore graphics_semaphore = render_forward_offscreen_pass(hdr_wait_semaphores);
	hdr_wait_semaphores            = VK_NULL_HANDLE;
	VkSemaphore post_semaphore     = render_compute_post(graphics_semaphore, compute_post_semaphore);
	compute_post_semaphore         = VK_NULL_HANDLE;
	VkSemaphore present_semaphore  = render_swapchain(post_semaphore);

    render_context->end_frame(present_semaphore);
}

void LightingSample::finish()
{
	// We're outside a frame context, so free the semaphore manually.
	device->wait_idle();
	vkDestroySemaphore(device->get_handle(), hdr_wait_semaphores, nullptr);


	if (compute_post_semaphore)
	{
		device->wait_idle();
		vkDestroySemaphore(device->get_handle(), compute_post_semaphore, nullptr);
	}
}

std::unique_ptr<clan::ClanSample> create_lighting()
{
	return std::make_unique<LightingSample>();
}

LightingSample::DepthMapSubpass::DepthMapSubpass(clan::RenderContext &render_context,
                                                 clan::ShaderSource &&vertex_shader, clan::ShaderSource &&fragment_shader,
                                                 clan::sg::Scene &scene, clan::sg::Camera &camera) :
    clan::ForwardSubpass(render_context, std::move(vertex_shader), std::move(fragment_shader), scene, camera)
{
	// PCF, so need depth bias to avoid (most) shadow acne.
	base_rasterization_state.depth_bias_enable = VK_TRUE;
}

void LightingSample::DepthMapSubpass::draw(clan::CommandBuffer &command_buffer)
{
	// Negative bias since we're using inverted Z.
	command_buffer.set_depth_bias(-1.0f, 0.0f, -2.0f);
	clan::ForwardSubpass::draw(command_buffer);
}

LightingSample::ShadowMapForwardSubpass::ShadowMapForwardSubpass(clan::RenderContext &render_context,
                                                                     clan::ShaderSource &&vertex_shader, clan::ShaderSource &&fragment_shader,
                                                                     clan::sg::Scene &scene, clan::sg::Camera &camera, clan::sg::Camera &shadow_camera_) :
    clan::ForwardSubpass(render_context, std::move(vertex_shader), std::move(fragment_shader), scene, camera),
    shadow_camera(shadow_camera_)
{
}

void LightingSample::ShadowMapForwardSubpass::set_shadow_map(const clan::ImageView *view, const clan::Sampler *sampler)
{
	shadow_view    = view;
	shadow_sampler = sampler;
}

void LightingSample::ShadowMapForwardSubpass::draw(clan::CommandBuffer &command_buffer)
{
	auto shadow_matrix = clan::vulkan_style_projection(shadow_camera.get_projection()) * shadow_camera.get_view();

	shadow_matrix = glm::translate(glm::vec3(0.5f, 0.5f, 0.0f)) * glm::scale(glm::vec3(0.5f, 0.5f, 1.0f)) * shadow_matrix;

	auto &render_frame = get_render_context().get_active_frame();

	auto allocation = render_frame.allocate_buffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(glm::mat4), thread_index);

	allocation.update(shadow_matrix);

	// Custom part, bind shadow map to the fragment shader.
	command_buffer.bind_buffer(allocation.get_buffer(), allocation.get_offset(), allocation.get_size(), 0, 5, 0);
	command_buffer.bind_image(*shadow_view, *shadow_sampler, 0, 6, 0);

	clan::ForwardSubpass::draw(command_buffer);
}

LightingSample::CompositeSubpass::CompositeSubpass(clan::RenderContext &render_context, clan::ShaderSource &&vertex_shader, clan::ShaderSource &&fragment_shader) :
    clan::Subpass(render_context, std::move(vertex_shader), std::move(fragment_shader))
{
}

void LightingSample::CompositeSubpass::set_texture(const clan::ImageView *hdr_view_, const clan::ImageView *bloom_view_,
                                                       const clan::Sampler *sampler_)
{
	hdr_view   = hdr_view_;
	bloom_view = bloom_view_;
	sampler    = sampler_;
}

void LightingSample::CompositeSubpass::prepare()
{
	auto &device   = render_context.get_device();
	auto &vertex   = device.get_resource_cache().request_shader_module(VK_SHADER_STAGE_VERTEX_BIT, get_vertex_shader());
	auto &fragment = device.get_resource_cache().request_shader_module(VK_SHADER_STAGE_FRAGMENT_BIT, get_fragment_shader());
	layout         = &device.get_resource_cache().request_pipeline_layout({&vertex, &fragment});
}

void LightingSample::CompositeSubpass::draw(clan::CommandBuffer &command_buffer)
{
	command_buffer.bind_image(*hdr_view, *sampler, 0, 0, 0);
	command_buffer.bind_image(*bloom_view, *sampler, 0, 1, 0);
	command_buffer.bind_pipeline_layout(*layout);

	// A depth-stencil attachment exists in the default render pass, make sure we ignore it.
	clan::DepthStencilState ds_state = {};
	ds_state.depth_test_enable      = VK_FALSE;
	ds_state.stencil_test_enable    = VK_FALSE;
	ds_state.depth_write_enable     = VK_FALSE;
	ds_state.depth_compare_op       = VK_COMPARE_OP_ALWAYS;
	command_buffer.set_depth_stencil_state(ds_state);

	command_buffer.draw(3, 1, 0, 0);
}