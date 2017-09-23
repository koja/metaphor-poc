#pragma once

#include "module/call_graph/include/call_graph.hpp"
#include "../include/double3.hpp"

#include <vector>

struct GraphLayoutSettings {
    int localNeighRadius; // default: 7
    int nIterations; // default: Harel & Koren 2002 use 4, but in my experience a larger number works better for some graphs, so I used 10

    GraphLayoutSettings(
        // WTF? co to ma znamenat
        int localNeighRadius_ = 1,
        int nIterations_ = 10
    ) {
        localNeighRadius = localNeighRadius_;
        nIterations = nIterations_;
    }
};

/**
 * @returns positions of vertices
 */
std::unordered_map<Function_Id, position3> harel_koren_layout_3d(const Call_Graph_Intf& graph, const GraphLayoutSettings& gls);
