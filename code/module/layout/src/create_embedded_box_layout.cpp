#include "../include/create_embedded_box_layout.hpp"

#include "../include/create_spiral_grid_layout.hpp"
#include "util/transform.hpp"

static std::map<box_id, size3> get_sizes_of(const std::map<box_id, size3>& sizes, const std::set<box_id>& ids) {
    std::map<box_id, size3> result;

    for(const auto& id : ids) {
        result.emplace( id, sizes.at(id) );
    }

    return result;
}

static std::map<box_id, size3> get_sizes(const Unique_Forest<box_id>& box_forest) {

    std::map<box_id, size3> sizes;

    // TODO vyresit degenerovane pripady
    auto visitor = [&sizes](const auto& forest, const box_id& value) {

        const auto children = forest.get_children(value);

        // it's a leaf!
        if( children.empty() ) {
            sizes.emplace( value, size3(10, 10, 10) );

        } else {

            const std::map<box_id, box3> layout =
                create_spiral_grid_layout(
                    get_sizes_of(sizes,children)
                );

            box3 bounding_box = layout.begin()->second;
            for(const auto& box : layout) {
                bounding_box = get_bounding_box(bounding_box, box.second);
            }

            sizes.emplace(value, bounding_box.size * 1.3 /* proportional margin */);
        }
    };

    box_forest.depth_first_search_post_order(visitor);

    return sizes;
}

std::map<box_id, box3> get_centered(const std::map<box_id, box3>& boxes) {

    // TODO horribly inefective
    box3 bounding_box = boxes.begin()->second;
    for(const auto& box : boxes) {
        bounding_box = get_bounding_box(bounding_box, box.second);
    }

    std::map<box_id, box3> result;

    for(auto& box : boxes) {
        result.emplace(
            box.first,
            box3(
                box.second.center - bounding_box.center,
                box.second.size
            )
        );
    }

    return result;
}

static std::map<box_id, box3> get_relative_positions(
    const Unique_Forest<box_id>& box_forest,
    const std::map<box_id, size3>& sizes
) {

    std::map<box_id, box3> relative_positions;

    auto visitor = [&sizes, &relative_positions](const auto& forest, const box_id& value) {
        const auto children = forest.get_children(value);

        // it's a leaf!
        if( children.empty() ) {
            // skip
        } else {
            merge_to(
                relative_positions,
                get_centered(
                    create_spiral_grid_layout(
                        get_sizes_of(sizes,children)
                    )
                )
            );
        }
    };

    box_forest.depth_first_search_post_order(visitor);

    // root nodes
    {
        merge_to(
            relative_positions,
            get_centered(
                create_spiral_grid_layout(
                    get_sizes_of( sizes, box_forest.get_roots() )
                )
            )
        );
    }

    return relative_positions;
}

// TODO DEBUG
#include <iostream>
std::map<box_id, box3> get_absolute_positions(
    const Unique_Forest<box_id>& box_forest,
    const std::map<box_id, box3>& relative_positions
) {
    std::map<box_id, box3> absolute_positions;

    std::cout << "box_forest " << std::endl << debug_string(box_forest) << std::endl;

    std::cout << "roots" << std::endl;
    for(const auto root : box_forest.get_roots()) {
        std::cout << root << std::endl;
    }
    std::cout << "-----" << std::endl;

    auto visitor = [&absolute_positions, &relative_positions](const auto& forest, const box_id& value) {

        std::cout << "visiting:" << value << std::endl;

        // root
        if( forest.get_roots().find(value) != forest.get_roots().end() ) {
            absolute_positions.emplace( value, relative_positions.at(value) );

        } else {
            const box_id& parent_box = forest.get_parent(value);
            absolute_positions.emplace(
                value,
                box3(
                    absolute_positions.at(parent_box).center + relative_positions.at(value).center,
                    relative_positions.at(value).size
                )
            );
        }
    };

    box_forest.depth_first_search_pre_order(visitor);

    return absolute_positions;
}

embedded_box_layout create_embedded_box_layout(const Unique_Forest<box_id>& box_forest) {
    const std::map<box_id, size3> sizes = get_sizes(box_forest);
    const std::map<box_id, box3> relative_positions = get_relative_positions(box_forest, sizes);
    const std::map<box_id, box3> absolute_positions = get_absolute_positions(box_forest, relative_positions);

    return embedded_box_layout(relative_positions, absolute_positions);
}
