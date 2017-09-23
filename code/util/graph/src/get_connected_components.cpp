#include "../include/get_connected_components.hpp"

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/strong_components.hpp>

std::map<Bi_Graph::Id, std::size_t> get_connected_components(/* const TODO this is hack */ Bi_Graph& _bi_graph) {
    
    // inspired by http://marko-editor.com/articles/graph_connected_components/
    
    /* WORKAROUND: 
     * Boost Graph does not have (1.64) tools for connected components of directed graph. 
     * The idea is to ensure that for every edge the opposite edge exists (by adding fake edges temporarily), use connected component tool from boost and delete fake edges.
     */
     
    std::vector<Bi_Graph::Edge_T> fake_edges;
     
    auto& boost_graph = _bi_graph.graph_;

    // add fake edges
    const auto edges = boost::edges(boost_graph);
    for (auto it = edges.first; it != edges.second; ++it) {
        const auto pair = boost::add_edge(
                boost::target(*it, boost_graph),
                boost::source(*it, boost_graph),
                boost_graph
            );
            
        fake_edges.push_back(pair.first);
    }

    // create result
    std::map<Bi_Graph::Id, std::size_t> result;
    {
        // note: boost graph uses zero-based component indexing
        std::vector<std::size_t> vertex_to_component_idx( boost::num_vertices(boost_graph) );

        boost::strong_components(boost_graph, make_iterator_property_map(vertex_to_component_idx.begin(), get(boost::vertex_index, boost_graph), vertex_to_component_idx[0]));

        // convert to Bi_Graph::Id - part of Bi_Graph public interface
        // TODO check, ze sedi indexy vertexu
        for(std::size_t vertex_idx = 0; vertex_to_component_idx.size(); ++vertex_idx) {
            result[ _bi_graph.vertex_to_id_.at(vertex_idx) ] = vertex_to_component_idx.at(vertex_idx);
        }
    }

    // revert - remove fake edges
    for(const auto& e : fake_edges) {
        boost::remove_edge(e, boost_graph);
    }    
        
    return result;
}
