#pragma once

#include "platform/platform.h"

namespace clan
{
    class WindowsPlatform : public Platform
    {
    public:
        WindowsPlatform(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        PSTR lpCmdLine, INT nCmdShow);

        virtual ~WindowsPlatform() = default;

        virtual const char *get_surface_extension() override;

    protected:
        virtual void create_window(const Window::Properties &properties) override;
    };
}