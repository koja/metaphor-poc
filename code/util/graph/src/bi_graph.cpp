#include "../include/bi_graph.hpp"
#include "../include/exception.hpp"

Bi_Graph::Id Bi_Graph::add_vertex() {
    const Vertex_T vertex = boost::add_vertex(graph_);
    const Id id = static_cast<Id>( id_to_vertex_.size() );
    
    id_to_vertex_.emplace(id, vertex);
    vertex_to_id_.emplace(vertex, id);
    
    return id;
}

void Bi_Graph::add_edge(const Id& _parent, const Id& _child) {
    if( boost::lookup_edge( id_to_vertex_.at(_parent), id_to_vertex_.at(_child), graph_ ).second ) {
        throw Duplicate_Edge{};
    }

    boost::add_edge( id_to_vertex_.at(_parent), id_to_vertex_.at(_child), graph_ );
}

std::unordered_set<Bi_Graph::Id> Bi_Graph::get_parents(const Id& _vertex_id) const {

    if( id_to_vertex_.find(_vertex_id) == id_to_vertex_.end() ) {
        throw Unknown_Value{};
    }

    const auto begin_end = boost::in_edges( id_to_vertex_.at(_vertex_id), graph_ );

    std::unordered_set<Bi_Graph::Id> result;

    for(auto it = begin_end.first; it != begin_end.second; ++it) {
        result.emplace(
            vertex_to_id_.at(
                boost::source(*it, graph_)
            )
        );
    }

    return result;
}

std::unordered_set<Bi_Graph::Id> Bi_Graph::get_children(const Id& _vertex_id) const {

    if( id_to_vertex_.find(_vertex_id) == id_to_vertex_.end() ) {
        throw Unknown_Value{};
    }
    
    const auto begin_end = boost::out_edges( id_to_vertex_.at(_vertex_id), graph_ );

    std::unordered_set<Bi_Graph::Id> result;

    for(auto it = begin_end.first; it != begin_end.second; ++it) {
        result.emplace(
            vertex_to_id_.at(
                boost::target(*it, graph_)
            )
        );
    }

    return result;
}

void Bi_Graph::foreach_edge(const Edge_Visitor_Functor& _visitor) const {

    typedef boost::graph_traits<BiDirected_Graph>::edge_iterator Edge_Iterator;
    
    std::pair<Edge_Iterator, Edge_Iterator> edge_iterator_range = boost::edges(graph_);
    
    for(Edge_Iterator edge_it = edge_iterator_range.first;
        edge_it != edge_iterator_range.second;
        ++edge_it
    ) {
        _visitor(
            *this,
            vertex_to_id_.at( boost::source(*edge_it, graph_) ),
            vertex_to_id_.at( boost::target(*edge_it, graph_) )
        );
    }
}

void Bi_Graph::foreach_vertex(const Vertex_Visitor_Functor& _visitor) const {

    typedef boost::graph_traits<BiDirected_Graph>::vertex_iterator Vertex_Iterator;
    
    // TODO vertices()?
    std::pair<Vertex_Iterator, Vertex_Iterator> vertex_iterator_range = boost::vertices(graph_);
    
    for(Vertex_Iterator vertex_it = vertex_iterator_range.first;
        vertex_it != vertex_iterator_range.second;
        ++vertex_it
    ) {
        _visitor(
            *this,
            vertex_to_id_.at( boost::vertex(*vertex_it, graph_) )
        );
    }
}
