#ifndef EMBEDDED_BOX_LAYOUT_435465566207
#define EMBEDDED_BOX_LAYOUT_435465566207

#include "double3.hpp"
#include "box_id.hpp"

#include <map>

struct embedded_box_layout {

    embedded_box_layout() {}

    explicit embedded_box_layout(
        const std::map<box_id, box3>& _relative_positions,
        const std::map<box_id, box3>& _absolute_positions
    ) :
        relative_positions(_relative_positions),
        absolute_positions(_absolute_positions)
    { }

    std::map<box_id, box3> relative_positions;
    std::map<box_id, box3> absolute_positions;
};

#endif
