#include "free_camera.h"

#include "common/error.h"
#include "common/glm_common.h"
#include "scene_graph/components/perspective_camera.h"
#include "scene_graph/components/transform.h"
#include "scene_graph/node.h"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace clan
{
    namespace sg
    {
        const float FreeCamera::ROTATION_MOVE_WEIGHT = 0.1f;

        const float FreeCamera::KEY_ROTATION_MOVE_WEIGHT = 0.5f;

        const float FreeCamera::TRANSLATION_MOVE_WEIGHT = 3.0f;

        const float FreeCamera::TRANSLATION_MOVE_STEP = 50.0f;

        const uint32_t FreeCamera::TRANSLATION_MOVE_SPEED = 4;

        FreeCamera::FreeCamera(Node &node) :
            NodeScript{node, "FreeCamera"}
        {}

        void FreeCamera::update(float delta_time)
        {
            glm::vec3 delta_translation(0.0f, 0.0f, 0.0f);
            glm::vec3 delta_rotation(0.0f, 0.0f, 0.0f);

            float mul_translation = speed_multiplier;

            if (key_pressed[KeyCode::W]){
                delta_translation.z -= TRANSLATION_MOVE_STEP;
            }
            if (key_pressed[KeyCode::S]){
                delta_translation.z += TRANSLATION_MOVE_STEP;
            }
            if (key_pressed[KeyCode::A]){
                delta_translation.x -= TRANSLATION_MOVE_STEP;
            }
            if (key_pressed[KeyCode::D]){
                delta_translation.x += TRANSLATION_MOVE_STEP;
            }
            if (key_pressed[KeyCode::Q]){
                delta_translation.y -= TRANSLATION_MOVE_STEP;
            }
            if (key_pressed[KeyCode::E]){
                delta_translation.y += TRANSLATION_MOVE_STEP;
            }
            if (key_pressed[KeyCode::LeftControl]){
                mul_translation *= (1.0f * TRANSLATION_MOVE_SPEED);
            }
            if (key_pressed[KeyCode::LeftShift]){
                mul_translation *= (1.0f / TRANSLATION_MOVE_SPEED);
            }

            if (key_pressed[KeyCode::I]){
                delta_rotation.x += KEY_ROTATION_MOVE_WEIGHT;
            }
            if (key_pressed[KeyCode::K]){
                delta_rotation.x -= KEY_ROTATION_MOVE_WEIGHT;
            }
            if (key_pressed[KeyCode::J]){
                delta_rotation.y += KEY_ROTATION_MOVE_WEIGHT;
            }
            if (key_pressed[KeyCode::L]){
                delta_rotation.y -= KEY_ROTATION_MOVE_WEIGHT;
            }

            if (mouse_button_pressed[MouseButton::Left] && mouse_button_pressed[MouseButton::Right]){
                delta_rotation.z += TRANSLATION_MOVE_WEIGHT * mouse_move_delta.x;
            }
            else if (mouse_button_pressed[MouseButton::Right]){
                delta_rotation.x -= ROTATION_MOVE_WEIGHT * mouse_move_delta.y;
                delta_rotation.y -= ROTATION_MOVE_WEIGHT * mouse_move_delta.x;
            }
            else if (mouse_button_pressed[MouseButton::Left]){
                delta_translation.x += TRANSLATION_MOVE_WEIGHT * mouse_move_delta.x;
                delta_translation.y += TRANSLATION_MOVE_WEIGHT * -mouse_move_delta.y;
            }

            delta_translation *= mul_translation * delta_time;
            delta_rotation *= delta_time;

            // Only re-calculate the transform if it's changed
            if (delta_rotation != glm::vec3(0.0f, 0.0f, 0.0f) || delta_translation != glm::vec3(0.0f, 0.0f, 0.0f)){
                auto &transform = get_node().get_component<Transform>();

                glm::quat qx = glm::angleAxis(delta_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
                glm::quat qy = glm::angleAxis(delta_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
                glm::quat orientation = glm::normalize(qy * transform.get_rotation() * qx);

                transform.set_translation(transform.get_translation() + delta_translation * glm::conjugate(orientation));
                transform.set_rotation(orientation);
            }

            mouse_move_delta = {};
        }

        void FreeCamera::input_event(const InputEvent &input_event)
        {
            if (input_event.get_source() == EventSource::Keyboard){
                const auto &key_event = static_cast<const KeyInputEvent &>(input_event);

                if (key_event.get_action() == KeyAction::Down || key_event.get_action() == KeyAction::Repeat){
                    key_pressed[key_event.get_code()] = true;
                }
                else{
                    key_pressed[key_event.get_code()] = false;
                }
            }
            else if (input_event.get_source() == EventSource::Mouse){
                const auto &mouse_button = static_cast<const MouseButtonInputEvent &>(input_event);

                glm::vec2 mouse_pos{std::floor(mouse_button.get_pos_x()), std::floor(mouse_button.get_pos_y())};

                if (mouse_button.get_action() == MouseAction::Down){
                    mouse_button_pressed[mouse_button.get_button()] = true;
                }
                if (mouse_button.get_action() == MouseAction::Up){
                    mouse_button_pressed[mouse_button.get_button()] = false;
                }
                if (mouse_button.get_action() == MouseAction::Move){
                    mouse_move_delta = mouse_pos - mouse_last_pos;
                    mouse_last_pos = mouse_pos;
                }
            }
        }

        void FreeCamera::resize(uint32_t width, uint32_t height)
        {
            auto &camera_node = get_node();

            if (camera_node.has_component<Camera>()){
                if (auto camera = dynamic_cast<PerspectiveCamera *>(&camera_node.get_component<Camera>())){
                    camera->set_aspect_ratio(static_cast<float>(width) / height);
                }
            }
        }
    }
}