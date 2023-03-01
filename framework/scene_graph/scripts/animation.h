#pragma once

#include <functional>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "scene_graph/components/transform.h"
#include "scene_graph/script.h"

namespace clan
{
    namespace sg
    {
        enum AnimationType
        {
            Linear,
            Step,
            CubicSpline
        };

        enum AnimationTarget
        {
            Translation,
            Rotation,
            Scale
        };

        struct AnimationSampler
        {
            AnimationType type{Linear};

            std::vector<float> inputs{};

            std::vector<glm::vec4> outputs{};
        };

        struct AnimationChannel
        {
            Node &node;

            AnimationTarget target;

            AnimationSampler sampler;
        };

        class Animation : public Script
        {
        public:
            Animation(const std::string &name = "");

            Animation(const Animation &);

            virtual void update(float delta_time) override;

            void update_times(float start_time, float end_time);

            void add_channel(Node &node, const AnimationTarget &target, const AnimationSampler &sampler);

        private:
            std::vector<AnimationChannel> channels;

            float current_time{0.0f};

            float start_time{std::numeric_limits<float>::max()};

            float end_time{std::numeric_limits<float>::min()};
        };
    }
}