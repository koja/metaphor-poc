#include "program_option_parsers.hpp"

#include <sstream>

namespace po = boost::program_options;

Options_Parser::Options_Parser(boost::program_options::options_description&& _options)
:   options_(_options)
{ }

std::string Options_Parser::get_options_description() const {

    std::stringstream ss;
    ss << options_;

    return ss.str();
}



static po::basic_parsed_options<char> parse_registered(
    const po::options_description& options,
    const std::vector<std::string>& args,
    po::variables_map& parsed_variables
) {
    auto parsed_options = po::basic_command_line_parser<char>(args).options(options).allow_unregistered().run();

    po::store(parsed_options, parsed_variables);

    po::notify(parsed_variables);
    
    return parsed_options;
}



Help_Parser::Help_Parser()
:   Options_Parser( po::options_description("Help option") )
{
    options_.add_options()
    (
        "help,h",
        "Produce help message"
    );
}

Parse_Result<bool> Help_Parser::is_help_requested(const std::vector<std::string>& args) const {
    po::variables_map parsed_variables;

    auto parsed_options = parse_registered(options_, args, parsed_variables);

    return {
        parsed_variables.count("help") + parsed_variables.count("h") > 0,
        po::collect_unrecognized(parsed_options.options, po::include_positional)
    };
}



Config_Options_Parser::Config_Options_Parser()
:   Options_Parser( po::options_description("Config options") )
{
    options_.add_options()
        (   "ast_list",
            po::value<std::string>()->default_value(std::string{}),
            "AST list filename"
        )
        (
            "crmap_list",
            po::value<std::string>()->default_value(std::string{}),
            "CR map list filename"
        );
}

Parse_Result<Config> Config_Options_Parser::parse_config(const std::vector<std::string>& args) const {
    po::variables_map parsed_variables;

    auto parsed_options = parse_registered(options_, args, parsed_variables);

    return {
        Config{
            parsed_variables.at("ast_list").as<std::string>(),
            parsed_variables.at("crmap_list").as<std::string>()
        },
        po::collect_unrecognized(parsed_options.options, po::include_positional)
    };
}



Unknown_Options_Parser::Unknown_Options_Parser()
:   Options_Parser( po::options_description("Unknown options") )
{ }

Parse_Result<bool> Unknown_Options_Parser::any_unknown_options(const std::vector<std::string>& args) const {
    po::variables_map parsed_variables;

    auto parsed_options = parse_registered(options_, args, parsed_variables);

    const std::vector<std::string> unrecognized_options = po::collect_unrecognized(parsed_options.options, po::include_positional);

    return {
        ! unrecognized_options.empty(),
        unrecognized_options
    };
}
