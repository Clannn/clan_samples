#include "frame_time_stats_provider.h"

namespace clan
{
    FrameTimeStatsProvider::FrameTimeStatsProvider(std::set<StatIndex> &requested_stats)
    {
        // We always, and only, support StatIndex::frame_times since it's handled directly by us.
        // Remove from requested set to stop other providers looking for it.
        requested_stats.erase(StatIndex::frame_times);
    }

    bool FrameTimeStatsProvider::is_available(StatIndex index) const
    {
        // We only support StatIndex::frame_times
        return index == StatIndex::frame_times;
    }

    StatsProvider::Counters FrameTimeStatsProvider::sample(float delta_time)
    {
        Counters res;
        // frame_times comes directly from delta_time
        res[StatIndex::frame_times].result = delta_time;
        return res;
    }
}