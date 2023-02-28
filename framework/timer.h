#pragma once

#include <chrono>

namespace clan
{
    class Timer
    {
    public:
        using Seconds      = std::ratio<1>;
        using Milliseconds = std::ratio<1, 1000>;
        using Microseconds = std::ratio<1, 1000000>;
        using Nanoseconds  = std::ratio<1, 1000000000>;

        using Clock             = std::chrono::steady_clock;
	    using DefaultResolution = Seconds;

        Timer();
        virtual ~Timer() = default;

        /**
         * @brief Starts the timer, elapsed() now returns the duration since start()
         */
        void start();

        /**
         * @brief Laps the timer, elapsed() now returns the duration since the last lap()
         */
	    void lap();

        /**
         * @brief Stops the timer, elapsed() now returns 0
         * @return The total execution time between `start()` and `stop()`
         */
        template<typename T = DefaultResolution>
        double stop()
        {
            if(!m_running){
                return 0;
            }
            m_running       = false;
            m_lapping       = false;
            auto duration = std::chrono::duration<double, T>(Clock::now() - m_start_time);
            m_start_time    = Clock::now();
            m_lap_time      = Clock::now();

            return duration.count();
        }

        /**
         * @brief Calculates the time difference between now and when the timer was started
         *        if lap() was called, then between now and when the timer was last lapped
         * @return The duration between the two time points (default in seconds)
         */
        template <typename T = DefaultResolution>
        double elapsed()
        {
            if (!m_running){
                return 0;
            }
            Clock::time_point start = m_start_time;
            if (m_lapping){
                start = m_lap_time;
            }
            return std::chrono::duration<double, T>(Clock::now() - start).count();
        }

        /**
         * @brief Calculates the time difference between now and the last time this function was called
         * @return The duration between the two time points (default in seconds)
         */
        template <typename T = DefaultResolution>
        double tick()
        {
            auto now      = Clock::now();
            auto duration = std::chrono::duration<double, T>(now - m_previous_tick);
            m_previous_tick = now;
            return duration.count();
        }

        /**
         * @brief Check if the timer is running
         */
        bool is_running() const;

    private:
        bool m_running{false};
        bool m_lapping{false};

        Clock::time_point m_start_time;
        Clock::time_point m_lap_time;
        Clock::time_point m_previous_tick;
    };
}