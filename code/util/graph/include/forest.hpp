#pragma once

// TODO budto cele prepsat nebo pokud neni potreba tak vyhodit

#include "exception.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/lookup_edge.hpp>
#include <boost/graph/depth_first_search.hpp>

#include <functional>

template<typename T> class Unique_Forest {

    public:
        void add_vertex(const T& _value);

        void add_edge(const T& _parent, const T& _child);

        const std::set<T>& get_roots() const;

        const T& get_parent(const T& _child) const;

        std::set<T> get_children(const T& _parent) const;

        typedef std::function<void(const Unique_Forest&, const T&)> Visitor_Functor;

        void depth_first_search_pre_order(const Visitor_Functor& _visitor) const;

        void depth_first_search_post_order(const Visitor_Functor& _visitor) const;

        // TODO visit subtree only
        // http://stackoverflow.com/a/22744953

    private:
        typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS> BiDirected_Graph;
        typedef boost::graph_traits<BiDirected_Graph>::vertex_descriptor Vertex_T;

        /**
         * Adaptor from public interface visitor (Visitor_Intf) to implementation visitor interface (boost::default_dfs_visitor)
         */
        friend struct Visitor_Adaptor;

        struct Visitor_Adaptor {
            Visitor_Adaptor(
                const Unique_Forest& _forest,
                const Visitor_Functor& _visitor
            ) :
                forest(_forest),
                visitor(_visitor)
            { }

            void visit(Vertex_T u, const BiDirected_Graph& g) const {
                visitor(forest, forest.vertex_to_value.at(u));
            }

            const Unique_Forest& forest;
            const Visitor_Functor& visitor;
        };

        struct Dfs_Pre_Visitor : boost::default_dfs_visitor {
            Dfs_Pre_Visitor(const Visitor_Adaptor& visitor)
            :   visitor_(visitor)
            { }

            void discover_vertex(Vertex_T u, const BiDirected_Graph& g) const {
                visitor_.visit(u, g);
            }

            const Visitor_Adaptor& visitor_;
        };

        struct Dfs_Post_Visitor : boost::default_dfs_visitor {
            Dfs_Post_Visitor(const Visitor_Adaptor& visitor)
            :   visitor_(visitor)
            { }

            void finish_vertex(Vertex_T u, const BiDirected_Graph& g) const {
                visitor_.visit(u, g);
            }

            const Visitor_Adaptor& visitor_;
        };

        BiDirected_Graph graph_;
        std::map<T, Vertex_T> value_to_vertex_;
        std::map<Vertex_T, T> vertex_to_value;
        std::set<T> roots_;
};

template<typename T> void Unique_Forest<T>::add_vertex(const T& _value) {
    if( value_to_vertex_.find(_value) != value_to_vertex_.end() ) {
        throw Duplicate_Value{};
    }

    const Vertex_T vertex = boost::add_vertex(graph_);
    value_to_vertex_.emplace(_value, vertex);
    vertex_to_value.emplace(vertex, _value);
    roots_.emplace(_value);
}

template<typename T> void Unique_Forest<T>::add_edge(const T& _parent, const T& _child) {
    if(
        boost::lookup_edge( value_to_vertex_.at(_parent), value_to_vertex_.at(_child), graph_ ).second
        ||
        boost::lookup_edge( value_to_vertex_.at(_child), value_to_vertex_.at(_parent), graph_ ).second
    ) {
        throw Duplicate_Edge{};
    }

    // TODO detect cyclic
    // http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/file_dependency_example.html#sec%3acycles

    if( value_to_vertex_.find(_parent) == value_to_vertex_.end() || value_to_vertex_.find(_child) == value_to_vertex_.end() ) {
        throw Unknown_Value{};
    }

    boost::add_edge( value_to_vertex_.at(_parent), value_to_vertex_.at(_child), graph_ );

    const auto it = roots_.find(_child);
    if( it != roots_.end() ) {
        roots_.erase(it);
    }
}

template<typename T> const std::set<T>& Unique_Forest<T>::get_roots() const {
    return roots_;
}

template<typename T> const T& Unique_Forest<T>::get_parent(const T& _child) const {
    return
        vertex_to_value.at(
            boost::source(
                *boost::in_edges( value_to_vertex_.at(_child), graph_ ).first,
                graph_
            )
        );
}

template<typename T> std::set<T> Unique_Forest<T>::get_children(const T& _parent) const {
    const auto begin_end = boost::out_edges( value_to_vertex_.at(_parent), graph_ );

    std::set<T> result;
    for(auto it = begin_end.first; it != begin_end.second; ++it) {
        result.emplace(
            vertex_to_value.at(
                boost::target(*it, graph_)
            )
        );
    }

    return result;
}

template<typename T> void Unique_Forest<T>::depth_first_search_pre_order(const Visitor_Functor& _visitor) const {

    std::vector<boost::default_color_type> color_map( boost::num_vertices(graph_) );

    for( const auto& a_root : roots_ ) {
        boost::depth_first_visit(
            graph_,
            value_to_vertex_.at(a_root),
            Dfs_Pre_Visitor( Visitor_Adaptor( *this, _visitor ) ),
            boost::make_iterator_property_map( color_map.begin(), get(boost::vertex_index, graph_) )
        );
    }
}

template<typename T> void Unique_Forest<T>::depth_first_search_post_order(const Visitor_Functor& _visitor) const {
    std::vector<boost::default_color_type> color_map( boost::num_vertices(graph_) );

    for( const auto& a_root : roots_ ) {
        boost::depth_first_visit(
            graph_,
            value_to_vertex_.at(a_root),
            Dfs_Post_Visitor( Visitor_Adaptor( *this, _visitor ) ),
            boost::make_iterator_property_map( color_map.begin(), get(boost::vertex_index, graph_) )
        );
    }
}

#include "util/debug_print.hpp"

template<typename T> struct Debug_String<Unique_Forest<T>> {
    static std::string get(const Unique_Forest<T>& forest) {
        std::string result;

        auto visitor =
            [&result] (const auto& forest, const T& value) {
                result += "vertex: " + debug_string(value) + "\n";
                for( const auto& child : forest.get_children(value) ) {
                    result += "edge: " + debug_string(value) + " to " + debug_string(child) + "\n";
                }
            };

        forest.depth_first_search_pre_order(visitor);

        return result;
    }
};
