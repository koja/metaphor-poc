#pragma once

#include <string>
#include <boost/program_options.hpp>

class Options_Parser {
    public:
        Options_Parser(boost::program_options::options_description&& _options);
        std::string get_options_description() const;

    protected:
        boost::program_options::options_description options_;
};

template<typename T_Return_Type> struct Parse_Result {
    typedef T_Return_Type Return_Type;
    
    Parse_Result(
        const Return_Type& _result,
        const std::vector<std::string>& _unrecognized_options
    ) :
        result(_result),
        unrecognized_options(_unrecognized_options)
    { }
    
    const Return_Type result;
    const std::vector<std::string> unrecognized_options;
};

class Help_Parser : public Options_Parser {
    public:
        Help_Parser();

        Parse_Result<bool> is_help_requested(const std::vector<std::string>& args) const;
};

#include "config.hpp"

class Config_Options_Parser : public Options_Parser {
    public:
        Config_Options_Parser();

        Parse_Result<Config> parse_config(const std::vector<std::string>& args) const;
};

class Unknown_Options_Parser : public Options_Parser {
    public:
        Unknown_Options_Parser();

        Parse_Result<bool> any_unknown_options(const std::vector<std::string>& args) const;
};
