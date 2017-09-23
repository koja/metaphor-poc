#include "../include/get_call_graph_data.hpp"

#include "visitors/call_visitor.hpp"
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace llvm;

void get_call_graph_data(
    Call_Graph_Builder_Intf& call_graph_builder,
    clang::ASTUnit* AST_unit
) {

    if(AST_unit == nullptr) {
        throw std::runtime_error("AST == nullptr");
    }
    Call_Visitor visitor(call_graph_builder);

    clang::ast_matchers::MatchFinder match_finder;
    visitor.register_to_finder(match_finder);

    match_finder.matchAST(AST_unit->getASTContext());
}
