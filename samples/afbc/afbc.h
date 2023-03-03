#pragma once

#include "rendering/render_pipeline.h"
#include "scene_graph/components/camera.h"
#include "timer.h"
#include "clan_sample.h"

/**
 * @brief Using framebuffer compression to reduce bandwidth
 */
class AFBCSample : public clan::ClanSample
{
  public:
	AFBCSample();

	virtual ~AFBCSample() = default;

	virtual bool prepare(clan::Platform &platform) override;

	virtual void update(float delta_time) override;

  private:
	clan::sg::Camera *camera{nullptr};

	virtual void draw_gui() override;

	void recreate_swapchain();

	bool afbc_enabled_last_value{false};

	bool afbc_enabled{false};

	std::chrono::system_clock::time_point start_time;
};

std::unique_ptr<clan::ClanSample> create_afbc();