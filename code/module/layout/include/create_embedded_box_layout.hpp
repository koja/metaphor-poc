#pragma once

#include "embedded_box_layout.hpp"
#include "util/graph/include/forest.hpp"

embedded_box_layout create_embedded_box_layout(const Unique_Forest<box_id>& box_forest);
