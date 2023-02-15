#pragma once

#include <unordered_map>

#include <CLI/CLI.hpp>

#include "help_formatter.h"
#include "platform/parser.h"
#include "platform/plugins/plugin.h"

namespace clan
{
    struct CLI11CommandContextState{
        std::string group_name = "";
    };

    class CLI11CommandContext: public CommandParserContext
    {
    public: 
        CLI11CommandContext(CLI::App* cli, const CLI11CommandContextState& state = {});

        virtual ~CLI11CommandContext() = default;

        bool has_group_name() const;
        const std::string& get_group_name() const;
        CLI11CommandContextState get_state() const;

        CLI::App* cli11;

    private: 
        CLI11CommandContextState m_state;
    };

    class CLI11CommandParser: public CommandParser
    {
    public:
        CLI11CommandParser(const std::string& name, const std::string& description, const std::vector<std::string>& args);
        virtual ~CLI11CommandParser() = default;

        virtual std::vector<std::string> help() const override;

        virtual bool parse(const std::vector<Plugin*>& plugins) override;

        virtual bool parse(const std::vector<Command*>& commands) override;

    protected:
        virtual void parse(CommandParserContext* context, CommandGroup* command) override;
        virtual void parse(CommandParserContext* context, SubCommand* command) override;
        virtual void parse(CommandParserContext* context, PositionalCommand* command) override;
        virtual void parse(CommandParserContext* context, FlagCommand* command) override;

        void parse(CLI11CommandContext* context, CommandGroup* command);
        void parse(CLI11CommandContext* context, SubCommand* command);
        void parse(CLI11CommandContext* context, PositionalCommand* command);
        void parse(CLI11CommandContext* context, FlagCommand* command);

        virtual bool contains(Command* command) const override;
    

    private: 
        std::vector<const char*> m_args;

        std::unique_ptr<CLI::App>                                m_cli11;
        std::unordered_map<Command*, CLI::Option*>               m_options;
        std::unordered_map<Command*, CLI::App*>                  m_sub_commands;
        std::unordered_map<Plugin*, std::shared_ptr<CLI::App>>   m_option_groups;
        std::shared_ptr<HelpFormatter>                           m_formatter;

        virtual std::vector<std::string> get_command_value(Command* command) const override;

        bool cli11_parse(CLI::App* app);
    };
}  //namespace clan