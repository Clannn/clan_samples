#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "common/error.h"
#include "common/glm_common.h"
#include "common/helpers.h"
#include "scene_graph/component.h"

namespace clan
{
    namespace sg
    {
        class Camera : public Component
        {
        public:
            Camera(const std::string &name);

            virtual ~Camera() = default;

            virtual std::type_index get_type() override;

            virtual glm::mat4 get_projection() = 0;

            glm::mat4 get_view();

            void set_node(Node &node);

            Node *get_node();

            const glm::mat4 get_pre_rotation();

            void set_pre_rotation(const glm::mat4 &pre_rotation);

        private:
            Node* node{nullptr};

            glm::mat4 pre_rotation{1.0f};
        };
    }
}