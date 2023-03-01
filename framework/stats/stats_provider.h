#pragma once

#include "stats_common.h"

#include <map>
#include <set>
#include <unordered_map>

namespace clan
{
    class CommandBuffer;

    /**
     * @brief Abstract interface for all StatsProvider classes
     */
    class StatsProvider
    {
    public:
        struct Counter
        {
            double result;
        };

        using Counters = std::unordered_map<StatIndex, Counter, StatIndexHash>;

        /**
         * @brief Virtual Destructor
         */
        virtual ~StatsProvider()
        {}

        /**
         * @brief Checks if this provider can supply the given enabled stat
         * @param index The stat index
         * @return True if the stat is available, false otherwise
         */
        virtual bool is_available(StatIndex index) const = 0;

        /**
         * @brief Retrieve graphing data for the given enabled stat
         * @param index The stat index
         */
        virtual const StatGraphData &get_graph_data(StatIndex index) const
        {
            return default_graph_map.at(index);
        }

        /**
         * @brief Retrieve default graphing data for the given stat
         * @param index The stat index
         */
        static const StatGraphData &default_graph_data(StatIndex index);

        /**
         * @brief Retrieve a new sample set
         * @param delta_time Time since last sample
         */
        virtual Counters sample(float delta_time) = 0;

        /**
         * @brief Retrieve a new sample set from continuous sampling
         * @param delta_time Time since last sample
         */
        virtual Counters continuous_sample(float delta_time)
        {
            return Counters();
        }

        /**
         * @brief A command buffer that we want stats about has just begun
         * @param cb The command buffer
         */
        virtual void begin_sampling(CommandBuffer &cb)
        {}

        /**
         * @brief A command buffer that we want stats about is about to be ended
         * @param cb The command buffer
         */
        virtual void end_sampling(CommandBuffer &cb)
        {}

    protected:
        static std::map<StatIndex, StatGraphData> default_graph_map;
    };
}