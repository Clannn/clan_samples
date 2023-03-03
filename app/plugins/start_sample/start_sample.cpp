#include "start_sample.h"

#include "apps.h"

#include <iostream>

namespace plugins
{
    StartSample::StartSample() :
        StartSampleTags("Apps",
                        "A collection of flags to samples and apps.",
                        {}, {&sample_subcmd, &samples_subcmd})
    {}

    bool StartSample::is_active(const clan::CommandParser &parser)
    {
        return parser.contains(&sample_cmd) || parser.contains(&samples_subcmd);
    }

    void StartSample::init(const clan::CommandParser &parser)
    {
        if (parser.contains(&sample_cmd))
        {
            // Launch Sample
            auto *sample = apps::get_sample(parser.as<std::string>(&sample_cmd));
            if (sample != nullptr)
            {
                clan::Window::OptionalProperties properties;
                std::string                     title = "Clan Samples: " + sample->name;
                properties.title                      = title;
                platform->set_window_properties(properties);
                platform->request_application(sample);
            }
        }
        else if (parser.contains(&samples_subcmd))
        {
            // List samples

            auto samples = apps::get_samples();

            LOGI("");
            LOGI("Available Samples");
            LOGI("");

            for (auto *app : samples)
            {
                auto sample = reinterpret_cast<apps::SampleInfo *>(app);
                LOGI("{}", sample->name.c_str());
                LOGI("\tid: {}", sample->id.c_str());
                LOGI("\tdescription: {}", sample->description.c_str());
                LOGI("");
            }

            platform->close();
        }
    }
}