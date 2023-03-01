#pragma once

#include <functional>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "scene_graph/components/transform.h"
#include "scene_graph/script.h"

/**
 *	@param std::shared_ptr<clan::sg::Transform> The transform to animate
 *  @param float The delta time of the frame to scale animations
 */
using TransformAnimFn = std::function<void(clan::sg::Transform &, float)>;

namespace clan
{
    namespace sg
    {
        class NodeAnimation : public NodeScript
        {
        public:
            NodeAnimation(Node &node, TransformAnimFn animation_fn);

            virtual ~NodeAnimation() = default;

            virtual void update(float delta_time) override;

            void set_animation(TransformAnimFn handle);

            void clear_animation();

        private:
            TransformAnimFn animation_fn{};
        };
    }
}