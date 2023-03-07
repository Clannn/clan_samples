#pragma once

#include "rendering/render_pipeline.h"
#include "rendering/subpasses/forward_subpass.h"
#include "scene_graph/components/camera.h"
#include "timer.h"
#include "clan_sample.h"

/**
 * @brief using multiple queues to async compute lighting
 */
class LightingSample : public clan::ClanSample
{
public:
    LightingSample();

    virtual ~LightingSample() = default;

    virtual bool prepare(clan::Platform& platform) override;

    virtual void update(float delta_time) override;

    virtual void finish() override;


private:
	clan::sg::Camera *camera{nullptr};
	clan::sg::Camera *shadow_camera{nullptr};

    virtual void draw_gui() override;

    std::chrono::system_clock::time_point start_time;

	void        render_shadow_pass();
	VkSemaphore render_forward_offscreen_pass(VkSemaphore hdr_wait_semaphore);
	VkSemaphore render_compute_post(VkSemaphore wait_graphics_semaphore, VkSemaphore wait_present_semaphore);
	VkSemaphore render_swapchain(VkSemaphore post_semaphore);

	void                                                prepare_render_targets();
	std::unique_ptr<clan::RenderTarget>                 forward_render_targets;
	std::unique_ptr<clan::RenderTarget>                 shadow_render_target;
	clan::RenderPipeline                                shadow_render_pipeline;
	clan::RenderPipeline                                forward_render_pipeline;
	std::unique_ptr<clan::Sampler>                      comparison_sampler;
	std::unique_ptr<clan::Sampler>                      linear_sampler;
	std::vector<std::unique_ptr<clan::Image>>           blur_chain;
	std::vector<std::unique_ptr<clan::ImageView>>       blur_chain_views;

	clan::PipelineLayout *threshold_pipeline{nullptr};
	clan::PipelineLayout *blur_down_pipeline{nullptr};
	clan::PipelineLayout *blur_up_pipeline{nullptr};

	const clan::Queue *early_graphics_queue{nullptr};
	const clan::Queue *present_graphics_queue{nullptr};
	const clan::Queue *post_compute_queue{nullptr};

	VkSemaphore hdr_wait_semaphores{};
	VkSemaphore compute_post_semaphore{};
	bool        rotate_shadows{true};

	struct DepthMapSubpass : clan::ForwardSubpass
	{
		DepthMapSubpass(clan::RenderContext &render_context,
		                clan::ShaderSource &&vertex_shader, clan::ShaderSource &&fragment_shader,
		                clan::sg::Scene &scene, clan::sg::Camera &camera);

		virtual void draw(clan::CommandBuffer &command_buffer) override;
	};

    struct ShadowMapForwardSubpass : clan::ForwardSubpass
	{
		ShadowMapForwardSubpass(clan::RenderContext &render_context,
		                        clan::ShaderSource &&vertex_shader, clan::ShaderSource &&fragment_shader,
		                        clan::sg::Scene &scene, clan::sg::Camera &camera, clan::sg::Camera &shadow_camera);

		void         set_shadow_map(const clan::ImageView *view, const clan::Sampler *sampler);

		virtual void draw(clan::CommandBuffer &command_buffer) override;

		const clan::ImageView *      shadow_view{nullptr};
		const clan::Sampler *        shadow_sampler{nullptr};
		clan::sg::Camera &           shadow_camera;
	};

	struct CompositeSubpass : clan::Subpass
	{
		CompositeSubpass(clan::RenderContext &render_context,
		                 clan::ShaderSource &&vertex_shader, clan::ShaderSource &&fragment_shader);
                         
		void set_texture(const clan::ImageView *hdr_view, const clan::ImageView *bloom_view,
		                         const clan::Sampler *sampler);

		virtual void draw(clan::CommandBuffer &command_buffer) override;

		virtual void prepare() override;

		const clan::ImageView *      hdr_view{nullptr};
		const clan::ImageView *      bloom_view{nullptr};
		const clan::Sampler *        sampler{nullptr};
		clan::PipelineLayout *       layout{nullptr};
	};

};

std::unique_ptr<clan::ClanSample> create_lighting();