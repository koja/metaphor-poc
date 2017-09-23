#pragma once

#include "module/call_graph/include/call_graph_builder.hpp"

#include <clang/ASTMatchers/ASTMatchFinder.h>

class Call_Visitor : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        explicit Call_Visitor(Call_Graph_Builder_Intf& _call_graph)
        :   call_graph_(_call_graph)
        { }

        void register_to_finder(clang::ast_matchers::MatchFinder& finder);

        void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

        Call_Graph_Builder_Intf& call_graph_;
};
