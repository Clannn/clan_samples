#include "base.h"

#include "common/vk_common.h"
#include "gltf_loader.h"
#include "gui.h"
#include "platform/platform.h"
#include "rendering/subpasses/forward_subpass.h"
#include "scene_graph/components/camera.h"

bool BaseSample::prepare(clan::Platform &platform)
{
	if (!ClanSample::prepare(platform))
	{
		return false;
	}


	load_scene("scenes/sponza/Sponza01.gltf");

	auto &camera_node = clan::add_free_camera(*scene, "main_camera", get_render_context().get_surface_extent());

	clan::ShaderSource vert_shader("base.vert");
	clan::ShaderSource frag_shader("base.frag");
	auto              scene_subpass = std::make_unique<clan::ForwardSubpass>(get_render_context(), std::move(vert_shader), std::move(frag_shader), *scene, camera_node.get_component<clan::sg::Camera>());

	auto render_pipeline = clan::RenderPipeline();
	render_pipeline.add_subpass(std::move(scene_subpass));

	set_render_pipeline(std::move(render_pipeline));

	gui = std::make_unique<clan::Gui>(*this, platform.get_window());

	return true;
}

void BaseSample::draw_gui()
{
	gui->show_options_window(
	    /* body = */ [this]() {},
	    /* lines= */ 0);
}

std::unique_ptr<clan::ClanSample> create_base()
{
	return std::make_unique<BaseSample>();
}