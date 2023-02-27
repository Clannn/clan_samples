#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "common/error.h"
#include "common/glm_common.h"
#include "scene_graph/script.h"

namespace clan
{
    namespace sg
    {
        class FreeCamera : public NodeScript
        {
        public:
            static const float ROTATION_MOVE_WEIGHT;

            static const float KEY_ROTATION_MOVE_WEIGHT;

            static const float TRANSLATION_MOVE_WEIGHT;

            static const float TRANSLATION_MOVE_STEP;

            static const uint32_t TRANSLATION_MOVE_SPEED;

            FreeCamera(Node& node);

            virtual ~FreeCamera() = default;

            virtual void update(float delta_time) override;

            virtual void input_event(const InputEvent &input_event) override;

            virtual void resize(uint32_t width, uint32_t height) override;

        private:
            float speed_multiplier{3.0f};

            glm::vec2 mouse_move_delta{0.0f};

            glm::vec2 mouse_last_pos{0.0f};

            std::unordered_map<KeyCode, bool> key_pressed;

            std::unordered_map<MouseButton, bool> mouse_button_pressed;
        };
    }
}