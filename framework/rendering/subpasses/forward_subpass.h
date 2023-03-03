#pragma once

#include "buffer_pool.h"
#include "rendering/subpasses/geometry_subpass.h"

// This value is per type of light that we feed into the shader
#define MAX_FORWARD_LIGHT_COUNT 8

namespace clan
{
    namespace sg
    {
        class Scene;
        class Node;
        class Mesh;
        class SubMesh;
        class Camera;
    }

    struct alignas(16) ForwardLights
    {
        Light directional_lights[MAX_FORWARD_LIGHT_COUNT];
        Light point_lights[MAX_FORWARD_LIGHT_COUNT];
        Light spot_lights[MAX_FORWARD_LIGHT_COUNT];
    };

    /**
     * @brief This subpass is responsible for rendering a Scene
     */
    class ForwardSubpass : public GeometrySubpass
    {
    public:
        /**
         * @brief Constructs a subpass designed for forward rendering
         * @param render_context Render context
         * @param vertex_shader Vertex shader source
         * @param fragment_shader Fragment shader source
         * @param scene Scene to render on this subpass
         * @param camera Camera used to look at the scene
         */
        ForwardSubpass(RenderContext &render_context, ShaderSource &&vertex_shader, ShaderSource &&fragment_shader, sg::Scene &scene, sg::Camera &camera);

        virtual ~ForwardSubpass() = default;

        virtual void prepare() override;

        /**
         * @brief Record draw commands
         */
        virtual void draw(CommandBuffer &command_buffer) override;
    };
}