#pragma once

#include "common/error.h"
#include "common/glm_common.h"
#include "scene_graph/components/camera.h"

namespace clan
{
    namespace sg
    {
        class OrthographicCamera : public Camera
        {
        public:
            OrthographicCamera(const std::string &name);

            OrthographicCamera(const std::string &name, float left, float right, float bottom, float top, float near_plane, float far_plane);

            virtual ~OrthographicCamera() = default;

            void set_left(float left);

            float get_left() const;

            void set_right(float right);

            float get_right() const;

            void set_bottom(float bottom);

            float get_bottom() const;

            void set_top(float top);

            float get_top() const;

            void set_near_plane(float near_plane);

            float get_near_plane() const;

            void set_far_plane(float far_plane);

            float get_far_plane() const;

            virtual glm::mat4 get_projection() override;

        private:
            float left{-1.0f};

            float right{1.0f};

            float bottom{-1.0f};

            float top{1.0f};

            float near_plane{0.0f};

            float far_plane{1.0f};
        };
    }
}