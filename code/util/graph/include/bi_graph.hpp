#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/lookup_edge.hpp>

#include "exception.hpp"

#include <functional>
#include <unordered_map>
#include <unordered_set>

class Bi_Graph {
    public:
        enum class Id : uint32_t { };

        Id add_vertex();

        void add_edge(const Id& _parent, const Id& _child);

        std::unordered_set<Id> get_parents(const Id& _vertex) const;
        
        std::unordered_set<Id> get_children(const Id& _vertex) const;

        typedef std::function<void(const Bi_Graph&, const Id& /* parent */, const Id& /* child */)> Edge_Visitor_Functor;
        void foreach_edge(const Edge_Visitor_Functor& _visitor) const;
        
        typedef std::function<void(const Bi_Graph&, const Id&)> Vertex_Visitor_Functor;
        void foreach_vertex(const Vertex_Visitor_Functor& _visitor) const;

    private:
        typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS> BiDirected_Graph;
        typedef boost::graph_traits<BiDirected_Graph>::vertex_descriptor Vertex_T;
        typedef boost::graph_traits<BiDirected_Graph>::edge_descriptor Edge_T;

        BiDirected_Graph graph_;
        std::unordered_map<Id, Vertex_T> id_to_vertex_;
        std::unordered_map<Vertex_T, Id> vertex_to_id_;
        
        friend std::map<Bi_Graph::Id, std::size_t> get_connected_components(/* const TODO this is hack */ Bi_Graph&);
};

#include "util/debug_print.hpp"

template<> struct Debug_String<Bi_Graph> {
    static std::string get(const Bi_Graph& graph) {
        std::string result;

        auto visitor =
            [&result] (const auto& Bi_Graph, const Bi_Graph::Id& parent, const Bi_Graph::Id& child) {
                result += debug_string( static_cast<std::size_t>(parent) ) + " -> " + debug_string( static_cast<std::size_t>(child) ) + "\n";
            };

        graph.foreach_edge(visitor);

        return result;
    }
};
