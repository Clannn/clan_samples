#pragma once

#include "stats_provider.h"

namespace clan
{
    class FrameTimeStatsProvider : public StatsProvider
    {
    public:
        /**
         * @brief Constructs a FrameTimeStatsProvider
         * @param requested_stats Set of stats to be collected. Supported stats will be removed from the set.
         */
        FrameTimeStatsProvider(std::set<StatIndex> &requested_stats);
        /**
         * @brief Checks if this provider can supply the given enabled stat
         * @param index The stat index
         * @return True if the stat is available, false otherwise
         */
        bool is_available(StatIndex index) const override;

        /**
         * @brief Retrieve a new sample set
         * @param delta_time Time since last sample
         */
        Counters sample(float delta_time) override;
    };
}