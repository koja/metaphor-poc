#include "../include/call_graph.hpp"

#include "module/clang_ast/include/get_function_info.hpp"

Call_Graph::Call_Graph(
    const Bi_Graph& _calls,
    const unordered_bimap<Function_Id, Vertex_Id>& _function_vertex_iso,
    const unordered_bimap<Vertex_Id, std::string>& _function_mangled_name_iso,
    const std::unordered_map<Vertex_Id, std::string>& _function_name,
    const std::unordered_map<Vertex_Id, std::unordered_map<Vertex_Id, std::size_t>>& _call_count
) :
    calls_(_calls),
    function_vertex_iso_(_function_vertex_iso),
    function_mangled_name_iso_(_function_mangled_name_iso),
    function_name_(_function_name),
    call_count_(_call_count)
{ }

void Call_Graph::for_each_function( std::function<void(const Function&)> visitor ) const {
    calls_.foreach_vertex(
        [this, visitor](const Bi_Graph& /* ignore */, const Vertex_Id& vertex_id) {
            visitor(
                get_function( function_vertex_iso_.at2(vertex_id) )
            );
        }
    );
}

void Call_Graph::for_each_call( std::function<void(const Function&, const Function&)> visitor ) const {
    calls_.foreach_edge(
        [this, visitor](const Bi_Graph& /* ignore */, const Vertex_Id& parent, const Vertex_Id& child) {
            visitor(
                get_function( function_vertex_iso_.at2(parent) ),
                get_function( function_vertex_iso_.at2(child) )
            );
        }
    );
}

std::size_t Call_Graph::function_count() const {
    return function_vertex_iso_.size();
}

Function Call_Graph::get_function(const Function_Id id) const {
    return Function {
        id,
        function_name_.at( function_vertex_iso_.at1(id) ),
        function_mangled_name_iso_.at1( function_vertex_iso_.at1(id) )
    };
}
