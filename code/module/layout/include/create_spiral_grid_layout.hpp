#pragma once

#include "box_id.hpp"
#include "double3.hpp"

#include <map>

std::map<box_id, box3> create_spiral_grid_layout(const std::map<box_id, size3>& _boxes);
