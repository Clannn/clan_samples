#pragma once

#include "clan_sample.h"
#include <memory>

/**
 * @brief Using 16-bit arithmetic extension to improve arithmetic throughput
 */
class KHR16BitArithmeticSample : public clan::ClanSample
{
  public:
	KHR16BitArithmeticSample();

	virtual ~KHR16BitArithmeticSample() = default;

	virtual bool prepare(clan::Platform &platform) override;

	virtual void request_gpu_features(clan::PhysicalDevice &gpu) override;

	virtual void draw_renderpass(clan::CommandBuffer &cmd, clan::RenderTarget &render_target) override;

  private:
	virtual void draw_gui() override;

	bool khr_16bit_arith_enabled{false};

	bool supported_extensions{false};
	bool supports_push_constant16{false};

	std::vector<clan::LoadStoreInfo>            load_store_infos;
	std::vector<std::unique_ptr<clan::Subpass>> subpasses;
	std::vector<VkClearValue>                  clear_values;

	std::unique_ptr<clan::Buffer>    blob_buffer;
	std::unique_ptr<clan::Image>     image;
	std::unique_ptr<clan::ImageView> image_view;
	std::unique_ptr<clan::Sampler>   sampler;
	clan::PipelineLayout *           compute_layout{nullptr};
	clan::PipelineLayout *           compute_layout_fp16{nullptr};

	unsigned frame_count{0};

	struct VisualizationSubpass : clan::Subpass
	{
		VisualizationSubpass(clan::RenderContext &context, clan::ShaderSource &&vertex_source, clan::ShaderSource &&fragment_source);
		virtual void prepare() override;
		virtual void draw(clan::CommandBuffer &command_buffer) override;

		clan::PipelineLayout *       layout{nullptr};
		const clan::ImageView *view{nullptr};
		const clan::Sampler *  sampler{nullptr};
	};
};

std::unique_ptr<clan::ClanSample> create_16bit();