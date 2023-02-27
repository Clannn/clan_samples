#include "camera.h"

#include "scene_graph/components/transform.h"
#include "scene_graph/node.h"

namespace clan
{
    namespace sg
    {
        Camera::Camera(const std::string &name) :
            Component{name}
        {}

        std::type_index Camera::get_type()
        {
            return typeid(Camera);
        }

        glm::mat4 Camera::get_view()
        {
            if (!node){
                throw std::runtime_error{"Camera component is not attached to a node"};
            }

            auto &transform = node->get_component<Transform>();
            return glm::inverse(transform.get_world_matrix());
        }

        void Camera::set_node(Node &n)
        {
            node = &n;
        }

        Node *Camera::get_node()
        {
            return node;
        }

        const glm::mat4 Camera::get_pre_rotation()
        {
            return pre_rotation;
        }

        void Camera::set_pre_rotation(const glm::mat4 &pr)
        {
            pre_rotation = pr;
        }
    }
}