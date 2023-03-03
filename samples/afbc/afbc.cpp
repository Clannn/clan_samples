#include "afbc.h"

#include "common/vk_common.h"
#include "gltf_loader.h"
#include "gui.h"
#include "platform/filesystem.h"
#include "platform/platform.h"
#include "rendering/subpasses/forward_subpass.h"
#include "stats/stats.h"

#include "scene_graph/components/camera.h"
#include "scene_graph/components/transform.h"
#include "scene_graph/node.h"

AFBCSample::AFBCSample()
{
	auto &config = get_configuration();

	config.insert<clan::BoolSetting>(0, afbc_enabled, false);
	config.insert<clan::BoolSetting>(1, afbc_enabled, true);
}

bool AFBCSample::prepare(clan::Platform &platform)
{
	if (!ClanSample::prepare(platform))
	{
		return false;
	}

	// We want AFBC disabled at start-up
	afbc_enabled = false;
	recreate_swapchain();

	load_scene("scenes/sponza/Sponza01.gltf");

	auto &camera_node = clan::add_free_camera(*scene, "main_camera", get_render_context().get_surface_extent());
	camera            = &camera_node.get_component<clan::sg::Camera>();

	clan::ShaderSource vert_shader("base.vert");
	clan::ShaderSource frag_shader("base.frag");
	auto              scene_subpass = std::make_unique<clan::ForwardSubpass>(get_render_context(), std::move(vert_shader), std::move(frag_shader), *scene, *camera);

	auto render_pipeline = clan::RenderPipeline();
	render_pipeline.add_subpass(std::move(scene_subpass));

	set_render_pipeline(std::move(render_pipeline));

	stats->request_stats({clan::StatIndex::gpu_ext_write_bytes});

	gui = std::make_unique<clan::Gui>(*this, platform.get_window(), stats.get());

	// Store the start time to calculate rotation
	start_time = std::chrono::system_clock::now();

	return true;
}

void AFBCSample::update(float delta_time)
{
	if (afbc_enabled != afbc_enabled_last_value)
	{
		recreate_swapchain();

		afbc_enabled_last_value = afbc_enabled;
	}

	/* Pan the camera back and forth. */
	auto &camera_transform = camera->get_node()->get_component<clan::sg::Transform>();

	float rotation_factor = std::chrono::duration<float>(std::chrono::system_clock::now() - start_time).count();

	glm::quat qy          = glm::angleAxis(0.003f * sin(rotation_factor * 0.7f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat orientation = glm::normalize(qy * camera_transform.get_rotation() * glm::angleAxis(0.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
	camera_transform.set_rotation(orientation);

	ClanSample::update(delta_time);
}

void AFBCSample::recreate_swapchain()
{
	std::set<VkImageUsageFlagBits> image_usage_flags = {VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT};

	if (!afbc_enabled)
	{
		// To force-disable AFBC, set an invalid image usage flag
		image_usage_flags.insert(VK_IMAGE_USAGE_STORAGE_BIT);
	}

	get_device().wait_idle();

	get_render_context().update_swapchain(image_usage_flags);
}

void AFBCSample::draw_gui()
{
	gui->show_options_window(
	    /* body = */ [this]() {
		    ImGui::Checkbox("Enable AFBC", &afbc_enabled);
	    },
	    /* lines = */ 1);
}

std::unique_ptr<clan::ClanSample> create_afbc()
{
	return std::make_unique<AFBCSample>();
}