#pragma once

#include "platform/plugins/plugin_base.h"

namespace plugins
{
    using StartSampleTags = clan::PluginBase<clan::tags::Entrypoint>;

    /**
     * @brief Start App
     * 
     * Loads a given sample
     * 
     * Usage: clan_sample sample afbc
     * 
     */
    class StartSample : public StartSampleTags
    {
    public:
        StartSample();

        virtual ~StartSample() = default;

        virtual bool is_active(const clan::CommandParser &parser) override;

        virtual void init(const clan::CommandParser &parser) override;

        clan::PositionalCommand sample_cmd     = {"sample_id", "ID of the sample to run"};
        clan::SubCommand        sample_subcmd  = {"sample", "Run a specific sample", {&sample_cmd}};
        clan::SubCommand        samples_subcmd = {"samples", "List available samples", {}};
    };
}