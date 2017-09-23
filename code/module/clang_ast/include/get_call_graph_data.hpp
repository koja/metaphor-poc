#pragma once

#include "module/call_graph/include/call_graph_builder.hpp"
#include <clang/Frontend/ASTUnit.h>

void get_call_graph_data(
    Call_Graph_Builder_Intf& call_graph_builder,
    clang::ASTUnit* AST_unit
);
