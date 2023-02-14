#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <typeindex>

namespace clan
{
    class Plugin;

    /**
      * @brief Abstract wrapper for implementing lists of commands. Useful for the CommandParser
     */
    class Command
    {
    public:
        Command(const std::string& name, const std::string& help_line);
        virtual ~Command() = default;

        /**
          * @brief Check that the command can be casted to another type of command
          * 
          * @tparam U - the type to cast to
          * @return true if the command can be casted
          * @return false if the command cannot be casted
         */
        template<typename U>
        bool is() const
        {
            return is_impl(typeid(U));
        }

        /**
          * @brief Case to a specific type of command
          * 
          * @tparam U  the type of command to cast to
          * @return U* - A ptr to this as the type of command
         */
        template<typename U>
        U* get()
        {
            assert(is<U>() && "Attempting to retrieve incorrect command type");
            return static_cast<U*>(this);
        }

        /**
          * @brief Case to a specific type of command
          * 
          * @tparam U the type of command to cast to
          * @return const U* - A ptr to this as the type of command
         */
        template<typename U>
        const U* get() const
        {
            assert(is<U>() && "Attempting to retrieve incorrect command type");
            return static_cast<const U*>(this);
        }

        const std::string& get_name() const;
        const std::string& get_help_line() const;
        void set_name(const std::string& name);
        void set_help_line(const std::string& help_line);

    protected:
        /**
         * @brief Implementation of the is method. See TypedCommand
         * 
         * @param index The type index to compare against
         * @return true Is equal to the given index
         * @return false Is not the equal to the given index
         */
	    virtual bool is_impl(const std::type_index &index) const = 0;

    private:
        std::string m_name;
        std::string m_help_line;
    };


    /**
      * @brief Allow a given command to implement subcommands
     */
    class MultipleCommands
    {
    public:
        MultipleCommands(const std::vector<Command*> &commands);
        virtual ~MultipleCommands() = default;

        const std::vector<Command*>& get_commands() const;

    private:
        std::vector<Command*> m_commands;
    };

    /**
     * @brief Implement type logic through is<CommandType>
     * 
     * @tparam Type - The type of the command
     */
    template<typename Type>
    class TypedCommand: public Command
    {
    public:
        TypedCommand(const std::string& name, const std::string& help_line):
            Command(name, help_line) {}

        virtual ~TypedCommand() = default;

    protected:
        virtual bool is_impl(const std::type_index& index) const override
        {
            return m_type == index;
        }

    private:
        std::type_index m_type = std::type_index(typeid(Type));
    };

    /**
     * @brief Command groups allow separate commands to be shown in a labeled group
     */
    class CommandGroup : public TypedCommand<CommandGroup>, public MultipleCommands
    {
    public:
        CommandGroup(const std::string &name, const std::vector<Command *> &commands);
        virtual ~CommandGroup() = default;
    };

    /**
     * @brief Subcommands act as separate entrypoints to the application and may implement a subset of commands
     */
    class SubCommand : public TypedCommand<SubCommand>, public MultipleCommands
    {
    public:
        SubCommand(const std::string &name, const std::string &help_line, const std::vector<Command *> &commands);
        virtual ~SubCommand() = default;
    };

    /**
     * @brief Positional commands are values taken from a given position in the command string
     */
    class PositionalCommand : public TypedCommand<PositionalCommand>
    {
    public:
        PositionalCommand(const std::string &name, const std::string &help_line);
        virtual ~PositionalCommand() = default;
    };

    enum class FlagType
    {
        FlagOnly,
        OneValue,
        ManyValues,
    };

    /**
     * @brief Flag command represent a flag and value e.g. --sample triangle
     */
    class FlagCommand: public TypedCommand<FlagCommand>
    {
    public:
        FlagCommand(FlagType type, const std::string &long_name, const std::string &short_name, const std::string &help_line);
        virtual ~FlagCommand() = default;

        FlagType get_flag_type() const;
        
    private:
        FlagType m_type;
    };

    /**
     * @brief Abstract context which different command parsers may use to pass their own specialized contexts
     */
    class CommandParserContext
    {
    public:
        CommandParserContext()          = default;
        virtual ~CommandParserContext() = default;
    };

    /**
     * @brief used to construct a CLI and parse command line arguments
     */
    class CommandParser
    {
    public:
        virtual ~CommandParser() = default;

        virtual bool contains(Command* command) const = 0;

        /**
         * @brief Cast a commands value to a given type
         * 
         * @tparam Type type to cast to
         * @param command Pointer to the command that should be casted
         * @return Type - A cast version of the commands
         */
        template <typename Type>
        Type as(Command* command) const
        {
            auto values = get_command_value(command);
            Type type{};
            bool implemented_type_conversion = convert_type(values, &type);
            assert(implemented_type_conversion && "Failed to retrieve value. Type unsupported");
            return type;
        }

        /**
         * @brief Retrieve the help menu generated by a parser implementation
         * 
         * @return std::vector<std::string> - A list of individual lines
         */
        virtual std::vector<std::string> help() const = 0;
        
        virtual bool parse(const std::vector<Plugin*>& plugins) = 0;

        virtual bool parse(const std::vector<Command*>& commands) = 0;

    protected:
        /**
         * @brief Individual parse functions visit each type of command to configure the underlying CLI implementation
         */
        virtual bool parse(CommandParserContext *context, const std::vector<Command*> &commands);
	    virtual void parse(CommandParserContext *context, CommandGroup *command) = 0;
	    virtual void parse(CommandParserContext *context, SubCommand *command) = 0;
	    virtual void parse(CommandParserContext *context, PositionalCommand *command) = 0;
	    virtual void parse(CommandParserContext *context, FlagCommand *command) = 0;

    private:
        /**
         * @brief Get the raw value parsed from command line arguments
         * 
         * @param command The command to retrieve the raw value from
         * 
         * @return std::vector<std::string> - The raw values for a given command
         */
        virtual std::vector<std::string> get_command_value(Command* command) const = 0;

        /**
         * @brief Cast from the CLI raw value to a given type
         * 
         * @tparam Type Type required
         * @param values The raw CLI values
         * @param type The required type to cast to
         * @return true - if implemented
         * @return false - if no implementation
         */
        template <typename Type>
        bool convert_type(const std::vector<std::string> & values, Type* type) const
        {
            return false;
        }

    };

    template <>
    inline bool CommandParser::convert_type(const std::vector<std::string> &values, uint32_t* type) const
    {
        if(values.size() != 1){
            *type = 0;
        }
        else{
            auto number = std::atoi(values[0].c_str());
            *type = static_cast<uint32_t>(number);
        }
        return true;
    }

    template<>
    inline bool CommandParser::convert_type(const std::vector<std::string> &values, float* type) const
    {
        if(values.size() != 1){
            *type = 0.0f;
        }
        else{
            *type = std::stof(values[0].c_str());
        }
        return true;
    }

    template <>
    inline bool CommandParser::convert_type(const std::vector<std::string> &values, std::vector<std::string> *type) const
    {
        *type = values;
        return true;
    }

    template <>
    inline bool CommandParser::convert_type(const std::vector<std::string> &values, std::string *type) const
    {
        if (values.size() > 0){
            *type = values[0];
        }
        else{
            *type = "";
        }
        return true;
    }
} //// namespace clan