#pragma once

#include <string>

#include "debug_info.h"
#include "platform/configuration.h"
#include "platform/input_events.h"
#include "timer.h"

namespace clan
{
    class Window;
    class Platform;

    class Application
    {
    public:
        Application();
        virtual ~Application() = default;

        /**
         * @brief Prepares the application for execution
         * @param platform The platform the application is being run on
         */
        virtual bool prepare(Platform &platform);

    	/**
         * @brief Updates the application
         * @param delta_time The time since the last update
         */
        virtual void update(float delta_time);

    	/**
         * @brief Handles cleaning up the application
         */
        virtual void finish();

    	/**
         * @brief Handles resizing of the window
         * @param width New width of the window
         * @param height New height of the window
         */
        virtual bool resize(const uint32_t width, const uint32_t height);

    	/**
         * @brief Handles input events of the window
         * @param input_event The input event object
         */
        virtual void input_event(const InputEvent &input_event);

        const std::string &get_name() const;

        void set_name(const std::string &name);

        DebugInfo &get_debug_info();

    protected:
        float m_fps{0.0f};
        float m_frame_time{0.0f};        // In ms
        
        uint32_t m_frame_count{0};
        uint32_t m_last_frame_count{0};

        Platform* m_platform;

    private:
        std::string m_name{};

        //The debug infor of the app
        DebugInfo debug_info{};
    };
}