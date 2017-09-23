#pragma once

#include "../include/view_factory.hpp"
#include "ogre3d/ogre_3d_view.hpp"

#include <memory>
#include <string>

/**
 * @return ogre3d view when view_name == "ogre3d"
 * @throw otherwise
 */
std::unique_ptr<View_Intf> create_view(const std::string& view_name) {
    if(view_name == "ogre3d") {
        return std::make_unique<Ogre_3D_View>();
    }
    
    throw std::runtime_error{"uknown view: " + view_name};
}
