#pragma once

#include <functional>
#include <unordered_map>

#include <CLI/Formatter.hpp>

namespace clan
{
    class HelpFormatter : public CLI::Formatter
    {
    public:
        struct Meta{
            std::string name;
            std::string description;
        };

        HelpFormatter() = default;
        HelpFormatter(const HelpFormatter&) = default;
        HelpFormatter(HelpFormatter&&) = default;

        virtual std::string make_help(const CLI::App*, std::string, CLI::AppFormatMode) const override;
        virtual std::string make_expanded(const CLI::App* sub) const override;

        void register_meta(const CLI::App* command, const Meta& meta);
        
    private:
        std::unordered_map<const CLI::App*, Meta> m_meta;

        const Meta* get_meta(const CLI::App* command) const;
    };
}  // namespace clan