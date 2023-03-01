#include "node_animation.h"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

#include "common/error.h"
#include "common/glm_common.h"
#include "scene_graph/components/perspective_camera.h"
#include "scene_graph/components/transform.h"
#include "scene_graph/node.h"

namespace clan
{
    namespace sg
    {
        NodeAnimation::NodeAnimation(Node &node, TransformAnimFn animation_fn) :
            NodeScript{node, ""},
            animation_fn{animation_fn}
        {
        }

        void NodeAnimation::update(float delta_time)
        {
            if (animation_fn)
            {
                animation_fn(get_node().get_component<Transform>(), delta_time);
            }
        }

        void NodeAnimation::set_animation(TransformAnimFn handle)
        {
            animation_fn = handle;
        }

        void NodeAnimation::clear_animation()
        {
            animation_fn = {};
        }
    }
}