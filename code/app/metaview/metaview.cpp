#include "module/clang_ast/include/ast.hpp"
#include "module/clang_ast/include/get_call_graph_data.hpp"

#include "module/layout/include/harel_koren_3d.hpp"

#include "module/view/include/view_factory.hpp"

#include "program_option_parsers.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static std::vector<std::string> read_AST_file_list(const std::string& filename) {
    std::vector<std::string> result;

    std::ifstream AST_list_src(filename);

    if( !AST_list_src.is_open() ) {
        throw std::runtime_error("failed to read AST file list");
    }

    std::clog << "input AST filename list: " << filename << std::endl;

    for (std::string line; std::getline( AST_list_src, line ); ) {
        result.emplace_back( line );
    }
    return result;
}

static void metaview(const Config& config) {
    std::unordered_map<
        std::string, // TODO filename
        std::unique_ptr<clang::ASTUnit>
    > AST_units;

    if( !config.ast_list_filename.empty() ) {
        for( const std::string& ast_filename : read_AST_file_list( config.ast_list_filename ) ) {
            auto ast_unit = load_AST(ast_filename);
            AST_units.emplace(ast_unit->getOriginalSourceFileName(), ast_unit.release() );
        }
        std::clog << "ASTS loaded." << std::endl;
    } else {
        std::clog << "No AST to load." << std::endl;
    }

    std::clog << "Creating call graphs." << std::endl;
    // TODO first = filename
    std::unordered_map<std::string, Call_Graph> unit_call_graphs;
    {
        for(const auto& ast_unit : AST_units) {
            Call_Graph_Builder call_graph_builder;
            get_call_graph_data( call_graph_builder, ast_unit.second.get() );

            unit_call_graphs.emplace( ast_unit.first, call_graph_builder.build() );
        }
    }
    std::clog << "Call graphs created." << std::endl;
    std::clog << "Call graphs count: " << unit_call_graphs.size() << std::endl;

    std::clog << "Calculating layouts." << std::endl;

    std::unordered_map<
        std::string,
        std::unordered_map<Function_Id, position3>
    > AST_unit_layouts;

    for(const auto& unit_call_graph : unit_call_graphs) {
        AST_unit_layouts.emplace(
            unit_call_graph.first,
            harel_koren_layout_3d(
                unit_call_graph.second,
                GraphLayoutSettings{1, 100}
            )
        );
    }
    std::clog << "Layouts calculated." << std::endl;

    std::unique_ptr<View_Intf> view = create_view("ogre3d");

    // TODO - tady se bude spojovat a for() a inter-obj cally resit
    if( !AST_unit_layouts.empty() ) {

        // TODO TMP
        const std::string compilation_unit = unit_call_graphs.cbegin()->first;
        const auto& call_graph = unit_call_graphs.at(compilation_unit);
        const auto& layout = AST_unit_layouts.at(compilation_unit);

        call_graph.for_each_function(
            [&view, &layout](const Function& f) {
                view->add_cube( layout.at(f.id), f.name );
                std::clog << "Adding cube to view for function " + f.name << std::endl;
            }
        );

        call_graph.for_each_call(
            [&view, &layout](const Function& caller, const Function& callee) {
                view->add_arrow( layout.at(caller.id), layout.at(callee.id) );
                std::clog << "Adding arrow to view for call by " + caller.name + " to " + callee.name << std::endl;
            }
        );
    }

    view->run();
}

int main(const int argc, const char* const argv[]) {

    Help_Parser h_p;
    Config_Options_Parser c_o_p;

    const auto get_args = [argc, argv] {
        std::vector<std::string> args;
        // skipping argv[0] (program name)
        for(std::size_t i = 1; i < argc; ++i) {
            args.emplace_back(
                std::string(
                    argv[i] != nullptr ? argv[i] : ""
                )
            );
        }
        return args;
    };
        
    const auto help_parse_result = h_p.is_help_requested( get_args() );

    if( help_parse_result.result ) {
        std::cout << h_p.get_options_description();
        std::cout << c_o_p.get_options_description();
        return EXIT_SUCCESS;
    }

    const auto config_parse_result = c_o_p.parse_config( help_parse_result.unrecognized_options );
    
    {
        Unknown_Options_Parser u_o_p;
            
        const auto unrecognized_options_parse_result = u_o_p.any_unknown_options( config_parse_result.unrecognized_options );
        if( unrecognized_options_parse_result.result ) {
            std::cerr << "Unrecognized options:\n";
            for( const auto& unrecognized : unrecognized_options_parse_result.unrecognized_options) {
                std::cerr << unrecognized << "\n";
            }
            std::cerr << h_p.get_options_description();
            std::cerr << c_o_p.get_options_description();
            
            return EXIT_FAILURE;
        }
    }

    metaview(config_parse_result.result);

    return 0;
}
