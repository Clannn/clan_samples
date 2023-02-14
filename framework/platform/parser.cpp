#include "parser.h"

namespace clan
{
    Command::Command(const std::string& name, const std::string& help_line):
        m_name(name), m_help_line(help_line)
    {}

    const std::string& Command::get_name() const
    {
        assert(!m_name.empty() && "Command name unset");
        return m_name;
    }

    const std::string& Command::get_help_line() const
    {
        assert(!m_help_line.empty() && "Command help line unset");
        return m_help_line;
    }

    void Command::set_name(const std::string& name)
    {
        m_name = name;
    }

    void Command::set_help_line(const std::string& help_line)
    {
        m_help_line = help_line;
    }
    
    MultipleCommands::MultipleCommands(const std::vector<Command*>& commands):
        m_commands(commands)
    {}

    const std::vector<Command*>& MultipleCommands::get_commands() const
    {
        return m_commands;
    }

    CommandGroup::CommandGroup(const std::string& name, const std::vector<Command*>& commands):
        TypedCommand<CommandGroup>(name, ""), MultipleCommands(commands)
    {}

    SubCommand::SubCommand(const std::string &name, const std::string &help_line, const std::vector<Command *> &comamnds) :
        TypedCommand<SubCommand>(name, help_line), MultipleCommands(comamnds)
    {}

    PositionalCommand::PositionalCommand(const std::string &name, const std::string &help_line) :
        TypedCommand<PositionalCommand>(name, help_line)
    {}

    FlagCommand::FlagCommand(FlagType type, const std::string &long_name, const std::string &short_name, const std::string &help_line) :
        TypedCommand<FlagCommand>("", help_line), m_type{type}
    {
        std::string name;
        if(!short_name.empty()){
            name += "-" + short_name;
        }
        if(!long_name.empty()){
            if(!name.empty()){
                name += ",";
            }
            name += "--" + long_name;
        }
        Command::set_name(name);
    }

    FlagType FlagCommand::get_flag_type() const
    {
        return m_type;
    }

    bool CommandParser::parse(const std::vector<Command*> &commands)
    {
        return parse(nullptr, commands);
    }

    bool CommandParser::parse(CommandParserContext* context, const std::vector<Command*> &commands)
    {
    #define PARSE(type)                          \
        if(command->is<type>()){                 \
            parse(context, command->get<type>());\
        }

        for(auto *command : commands){
            PARSE(SubCommand);
            PARSE(PositionalCommand);
            PARSE(FlagCommand);
            PARSE(CommandGroup);
        }
    #undef PARSE

        return true;
    }
}  //namespace clan