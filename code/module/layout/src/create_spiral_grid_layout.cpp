#include "../include/create_spiral_grid_layout.hpp"

#include <boost/range/adaptor/reversed.hpp>

namespace {
	struct size3_less {
		bool operator() (const size3& lhs, const size3& rhs) const {
			return
				lhs.z != rhs.z
					?   lhs.z < rhs.z
					:   lhs.y != rhs.y
							?   lhs.y < rhs.y
							:   lhs.x < rhs.x;
		}
	};
}

static position3 get_next_center_position(const box3& current_bounding_box, const size3& next_box_size ) {
    // TODO parametrised margin
    const double margin_coeff = 1.05;
    const size3 margin(next_box_size * margin_coeff);
    const double x_to_y_treshold = 2.0;
    const double y_to_z_treshold = 1.5;

    const size3 bb_size = current_bounding_box.size;
    const position3 max = get_max(current_bounding_box);

    const size3 next_box_zero_based_center = next_box_size * 0.5;
    const position3 next_box_center{next_box_zero_based_center.x, next_box_zero_based_center.y, next_box_zero_based_center.z};

    if( current_bounding_box.size == size3{0, 0, 0} ) {
        return next_box_center;

    } else if (bb_size.x / bb_size.y < x_to_y_treshold) {   // TODO parametrised treshold
        return position3{max.x + margin.x, 0, 0} + next_box_center;

    } else if (bb_size.y / bb_size.z < y_to_z_treshold) {   // TODO parametrised treshold
        return position3{0, max.y + margin.y, 0} + next_box_center;

    } else {
        return position3{0, 0, max.z + margin.z} + next_box_center;
    }
}

std::map<box_id, box3> create_spiral_grid_layout(const std::map<box_id, size3>& _boxes) {

    if( _boxes.empty() ) {
        return {};
    }

    std::multimap<size3, box_id, size3_less> size_ordered_boxes;

    for(const auto& box : _boxes) {
        size_ordered_boxes.emplace(box.second, box.first);
    }

    std::map<box_id, box3> result;

    box3 bounding_box(
        position3{0, 0, 0},
        size3{0, 0, 0}
    );

    for(const auto& box : boost::adaptors::reverse(size_ordered_boxes) ) {
        const box3 next_box(
            get_next_center_position(bounding_box, box.first),
            box.first
        );
        result.emplace(box.second, next_box);

        bounding_box = get_bounding_box(bounding_box, next_box);
    }

    return result;
}
