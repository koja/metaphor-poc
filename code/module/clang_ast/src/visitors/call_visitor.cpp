#include "call_visitor.hpp"

#include "clang/ASTMatchers/ASTMatchers.h"

// TODO debug log
#include <iostream>

namespace {
    using namespace clang;
    using namespace clang::ast_matchers;
}

void Call_Visitor::register_to_finder(clang::ast_matchers::MatchFinder& finder) {
     finder.addMatcher(
        callExpr(
            //isExpansionInMainFile(),
            callee( functionDecl() ),
            hasAncestor(
                functionDecl().bind("caller")
            )
        ).bind("callee"),
        this
    );
}

void Call_Visitor::run(const MatchFinder::MatchResult& result) {

    const clang::FunctionDecl* callee = 
        dynamic_cast<const clang::FunctionDecl*>(
            result.Nodes.getNodeAs<CallExpr>("callee")->getCalleeDecl()
        );

    if(callee == nullptr) { throw "callee is NULL"; /* TODO */ }

    // TODO FIX LOGGING LOG
    std::clog <<
        "visiting call: " <<
        result.Nodes.getNodeAs<clang::FunctionDecl>("caller")->getNameAsString() <<
        "() ->" <<
        callee->getNameAsString() <<        
        "()" <<
        std::endl;

    call_graph_.add_call(
        result.Nodes.getNodeAs<clang::FunctionDecl>("caller"),
        callee
    );
}
