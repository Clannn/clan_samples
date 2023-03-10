#include "plugin.h"
#include "platform/platform.h"

namespace clan
{
    std::vector<Plugin *> associate_plugins(const std::vector<Plugin *> &plugins)
    {
        for (auto *plugin : plugins)
        {
            for (auto *comparison_plugin : plugins)
            {
                bool full_control = comparison_plugin->has_tags<tags::FullControl>();
                bool stopping     = comparison_plugin->has_tags<tags::Stopping>();
                bool controlling  = full_control || stopping;

                bool entrypoint = comparison_plugin->has_tags<tags::Entrypoint>();

                if (plugin->has_tag<tags::FullControl>() && (controlling || entrypoint))
                {
                    plugin->excludes(comparison_plugin);
                    continue;
                }

                if (plugin->has_tag<tags::Stopping>() && stopping)
                {
                    plugin->excludes(comparison_plugin);
                    continue;
                }

                if (plugin->has_tag<tags::Entrypoint>() && entrypoint)
                {
                    plugin->excludes(comparison_plugin);
                    continue;
                }

                plugin->includes(comparison_plugin);
            }
        }

        return plugins;
    }

    bool Plugin::activate_plugin(Platform *p, const CommandParser &parser, bool force_activation)
    {
        LOGI("activate plugin");
        platform = p;

        bool active = is_active(parser);

        // Plugin activated
        if (force_activation || active)
        {
            LOGI("init plugin");
            init(parser);
        }

        return active;
    }

    const std::string &Plugin::get_name() const
    {
        return name;
    }

    const std::string &Plugin::get_description() const
    {
        return description;
    }

    void Plugin::excludes(Plugin *plugin)
    {
        exclusions.push_back(plugin);
    }

    const std::vector<Plugin *> &Plugin::get_exclusions() const
    {
        return exclusions;
    }

    void Plugin::includes(Plugin *plugin)
    {
        inclusions.push_back(plugin);
    }

    const std::vector<Plugin *> &Plugin::get_inclusions() const
    {
        return inclusions;
    }
}