#include "timer.h"

namespace clan
{
    Timer::Timer():
        m_start_time(Clock::now()), m_previous_tick(Clock::now())
    {}

    void Timer::start()
    {
        if(!m_running){
            m_running = true;
            m_start_time = Clock::now();
        }
    }

    void Timer::lap()
    {
        m_lapping = true;
        m_lap_time = Clock::now();
    }

    bool Timer::is_running() const
    {
        return m_running;
    }

}