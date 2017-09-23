#include "../include/call_graph_builder.hpp"

#include "module/clang_ast/include/get_function_info.hpp"

void Call_Graph_Builder::add_function(const clang::FunctionDecl* const function) {
    if( function_vertex_iso_.get_1_to_2().find(function) == function_vertex_iso_.get_1_to_2().cend() ) {
        const Vertex_Id vertex_id = calls_.add_vertex();

        function_vertex_iso_.emplace(function, vertex_id);
        function_name_.emplace(vertex_id, get_function_name(*function));
        function_mangled_name_iso_.emplace(vertex_id, get_function_name(*function));
    }
}

void Call_Graph_Builder::add_call(const clang::FunctionDecl* const callee, const clang::FunctionDecl* const caller) {

    add_function(caller);
    add_function(callee);

    const Vertex_Id callee_id = function_vertex_iso_.at1(callee);
    const Vertex_Id caller_id = function_vertex_iso_.at1(caller);

    calls_.add_edge(callee_id, caller_id);
    call_count_[callee_id][caller_id] += 1;
}

Call_Graph Call_Graph_Builder::build() const {
    const auto get_function_vertex_iso = [this](){
        unordered_bimap<Function_Id, Vertex_Id> tmp;
        for(const auto& e : function_vertex_iso_.get_1_to_2() ) {
            tmp.emplace(
                Function_Id{
                    reinterpret_cast<std::size_t>( e.first )
                },
                e.second
            );
        }
        return tmp;
    };

    return Call_Graph(
        calls_,
        get_function_vertex_iso(),
        function_mangled_name_iso_,
        function_name_,
        call_count_
    );
}
