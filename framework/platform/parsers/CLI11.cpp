#include "CLI11.h"
#include "common/logging.h"
#include "common/strings.h"

namespace clan
{
    CLI11CommandContext::CLI11CommandContext(CLI::App *cli, const CLI11CommandContextState &state):
        CommandParserContext(), cli11(cli), m_state(state)
    {}

    bool CLI11CommandContext::has_group_name() const
    {
        return !m_state.group_name.empty();
    }

    const std::string &CLI11CommandContext::get_group_name() const
    {
        return m_state.group_name;
    }

    CLI11CommandContextState CLI11CommandContext::get_state() const
    {
        return m_state;
    }

    CLI11CommandParser::CLI11CommandParser(const std::string &name, const std::string &description, const std::vector<std::string> &args) :
        m_cli11{std::make_unique<CLI::App>(description, name)}, m_formatter{std::make_shared<HelpFormatter>()}
    {
        m_cli11->formatter(m_formatter);

        m_args.resize(args.size());
        std::transform(args.begin(), args.end(), m_args.begin(), [](const std::string &string) -> char * { return const_cast<char *>(string.c_str()); });
    }

    std::vector<std::string> CLI11CommandParser::help() const
    {
        return split(m_cli11->help(), "\n");
    }

    // Helper to reduce duplication - throw should not occur as there should always be a valid context passed
	void CLI11CommandParser::parse(CommandParserContext *context, CommandGroup *command)
	{
		parse(context == nullptr ? throw : dynamic_cast<CLI11CommandContext *>(context), command);
	}
    void CLI11CommandParser::parse(CommandParserContext *context, SubCommand *command)
	{
		parse(context == nullptr ? throw : dynamic_cast<CLI11CommandContext *>(context), command);
	}
    void CLI11CommandParser::parse(CommandParserContext *context, PositionalCommand *command)
	{
		parse(context == nullptr ? throw : dynamic_cast<CLI11CommandContext *>(context), command);
	}
    void CLI11CommandParser::parse(CommandParserContext *context, FlagCommand *command)
	{
		parse(context == nullptr ? throw : dynamic_cast<CLI11CommandContext *>(context), command);
	}

    void CLI11CommandParser::parse(CLI11CommandContext *context, CommandGroup *command)
    {
        auto state       = context->get_state();
        state.group_name = command->get_name();
        CLI11CommandContext group_context(context->cli11, state);
        CommandParser::parse(&group_context, command->get_commands());
    }

    void CLI11CommandParser::parse(CLI11CommandContext *context, SubCommand *command)
    {
        auto *subcommand       = context->cli11->add_subcommand(command->get_name(), command->get_help_line());
        m_sub_commands[command] = subcommand;
        subcommand->formatter(m_formatter);
        CLI11CommandContext subcommand_context(subcommand, context->get_state());
        CommandParser::parse(&subcommand_context, command->get_commands());
    }

    void CLI11CommandParser::parse(CLI11CommandContext *context, PositionalCommand *command)
    {
        auto *option = context->cli11->add_option(command->get_name(), command->get_help_line());
        m_options.emplace(command, option);
        if (context->has_group_name()){
            option->group(context->get_group_name());
        }
    }

    void CLI11CommandParser::parse(CLI11CommandContext *context, FlagCommand *command)
    {
        CLI::Option *flag;

        switch (command->get_flag_type()){
            case FlagType::FlagOnly:
                flag = context->cli11->add_flag(command->get_name(), command->get_help_line());
                break;
            case FlagType::OneValue:
            case FlagType::ManyValues:
                flag = context->cli11->add_option(command->get_name(), command->get_help_line());
                break;
        }

        m_options.emplace(command, flag);
        if (context->has_group_name()){
            flag->group(context->get_group_name());
        }
    }

    bool CLI11CommandParser::contains(Command *command) const
    {
        {
            auto it = m_options.find(command);
            if (it != m_options.end()){
                return it->second->count() > 0;
            }
        }
        {
            auto it = m_sub_commands.find(command);
            if (it != m_sub_commands.end()){
                return it->second->count() > 0;
            }
        }
        return false;
    }

    std::vector<std::string> CLI11CommandParser::get_command_value(Command *command) const
    {
        auto it = m_options.find(command);

        if (it == m_options.end())
        {
            return {};
        }

        return it->second->results();
    }

    bool CLI11CommandParser::parse(const std::vector<Plugin *> &plugins)
    {
        // Generate all command groups
        for (auto plugin : plugins){
            auto group = std::make_unique<CLI::App>();
            m_formatter->register_meta(group.get(), {plugin->get_name(), plugin->get_description()});
            CLI11CommandContext context(group.get());
            CommandParser::parse(&context, plugin->get_cli_commands());
            m_option_groups[plugin] = std::move(group);
        }

        // Associate correct command groups
        for (auto plugin : plugins){
            auto plugin_cli       = m_option_groups[plugin];
            auto included_plugins = plugin->get_inclusions();
            auto commands         = plugin->get_cli_commands();
            for (auto command : commands){
                // Share flags and options with sub commands
                if (command->is<SubCommand>()){
                    auto cli11_sub_command = m_sub_commands[command];
                    for (auto included_plugin : included_plugins){
                        cli11_sub_command->add_subcommand(m_option_groups[included_plugin]);
                    }
                }
            }
            m_cli11->add_subcommand(plugin_cli);
        }

        return cli11_parse(m_cli11.get());
    }

    bool CLI11CommandParser::parse(const std::vector<Command *> &commands)
    {
        CLI11CommandContext context(m_cli11.get());
        if (!CommandParser::parse(&context, commands)){
            return false;
        }

        return cli11_parse(m_cli11.get());
    }

    bool CLI11CommandParser::cli11_parse(CLI::App *app)
    {
        try{
            m_args.insert(m_args.begin(), "clan_samples");
            app->parse(static_cast<int>(m_args.size()), m_args.data());
        }
        catch (CLI::CallForHelp e){
            return false;
        }
        catch (CLI::ParseError e){
            bool success = e.get_exit_code() == static_cast<int>(CLI::ExitCodes::Success);
            if (!success){
                LOGE("CLI11 Parse Error: [{}] {}", e.get_name(), e.what());
                return false;
            }
        }
        return true;
    }
}