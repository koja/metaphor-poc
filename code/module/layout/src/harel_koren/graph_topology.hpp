#pragma once

#include "../util/random.hpp"

#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>

// TODO
typedef std::size_t V_Id;
typedef std::size_t Vert_Idx;
typedef std::size_t Topo_Dist;

// std::vector<T> is implicitly indexed std::map<std::size_t, T>
// represent std::size_t x std::size_t 2D array as std::size_t^2 long 1D array
// TODO optimalizace maji byt napred "radky" nebo "sloupce"? kvuli cachovani
typedef std::pair< std::vector<Topo_Dist>, std::size_t > Distance_Matrix;

inline Distance_Matrix create(const std::size_t N, const Topo_Dist init_value = 0) {
    return std::make_pair(
        std::vector<Topo_Dist>(N*N, init_value),
        N
    );
}

inline std::size_t get_N(const Distance_Matrix& matrix) {
    return matrix.second;
}

inline Topo_Dist& get_ref(Distance_Matrix& matrix, const V_Id i, const V_Id j) {
    // DEBUG version with bounds checking
    return matrix.first.at(i * matrix.second + j);
}

inline Topo_Dist get_val(const Distance_Matrix& matrix, const V_Id i, const V_Id j) {
    // DEBUG version with bounds checking
    return matrix.first.at(i * matrix.second + j);
}



/**
 * https://en.wikipedia.org/wiki/Dijkstra's_algorithm
 * Vertexes are implicitly represented as sequence of identifiers with value in [ 0; vertex_count - 1 ]
 */
inline Distance_Matrix get_shortest_paths(const std::size_t vertex_count, const std::map<V_Id, std::set<V_Id>>& edges) {
    // TODO assert, ze rozmery vstupu sedi
    Distance_Matrix result = create(vertex_count, 0);

    std::queue<V_Id> to_be_visited;

    for(V_Id start = 0; start < vertex_count; ++start) {

        to_be_visited.push(start);

        while( !to_be_visited.empty() ) {

            const V_Id current_v = to_be_visited.front();
            to_be_visited.pop();

            const Topo_Dist current_path_lenght = get_val(result, start, current_v);

            for(const V_Id vertex : edges.at(current_v)) {

                if( vertex == start ) { /* leaving 0 distance by which it was initialized */ continue; }

                Topo_Dist& result_start_vertex = get_ref(result, start, vertex);

                if( result_start_vertex > 0) { /* already discovered */ continue; }

                // TODO optimalizace - vyuzit symetrie
                result_start_vertex = current_path_lenght + 1;
                to_be_visited.push(vertex);
            }
        }
    }

    return result;
}

inline Topo_Dist get_max_distance_in_set(const Distance_Matrix& shortest_paths, const std::vector<V_Id>& vertex_set) {

    Topo_Dist candidate = 0;

    for(V_Id i = 0; i < vertex_set.size(); i++) {
        for(V_Id j = 0; j < i; j++) {
            candidate = std::max(
                candidate,
                get_val( shortest_paths, vertex_set.at(i), vertex_set.at(j) )
            );
        }
    }

    return candidate;
}

inline std::map<Vert_Idx, std::set<Vert_Idx>> get_close_neighbours(
    const std::vector<V_Id>& vertexes,
    const std::size_t max_dist,
    const Distance_Matrix& shortest_paths
){
    std::map<Vert_Idx, std::set<Vert_Idx>> close_neighbours;

    for(std::size_t v_i = 0; v_i < vertexes.size(); ++v_i) {
        // create empty sets if neighbour is not close enough
        close_neighbours[v_i] = std::set<Vert_Idx>{};

        for(std::size_t u_i = 0; u_i < v_i /* distance is symmetric */; ++u_i) {
            if( get_val(shortest_paths, vertexes[v_i], vertexes[u_i]) <= max_dist ) {
                close_neighbours[v_i].insert(u_i);
                close_neighbours[u_i].insert(v_i);
            }
        }
    }

    return close_neighbours;
}
