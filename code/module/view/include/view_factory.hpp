#pragma once

#include "../include/view_intf.hpp"

#include <memory>
#include <string>

/**
 * @return ogre3d view when view_name == "ogre3d"
 * @throw otherwise
 */
std::unique_ptr<View_Intf> create_view(const std::string& view_name);
