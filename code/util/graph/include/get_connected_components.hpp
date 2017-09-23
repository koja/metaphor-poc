#pragma once

#include "bi_graph.hpp"

#include <vector>

/**
 * @returns mapping from vertex to (zero-based) component idx
 */
std::map<Bi_Graph::Id, std::size_t> get_connected_components(/* const TODO this is hack */Bi_Graph&);
