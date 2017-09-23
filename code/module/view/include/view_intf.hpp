#pragma once

#include "module/layout/include/double3.hpp"

#include <memory>
#include <string>
#include <vector>

enum class Cube_Id : uint32_t { };

struct View_Intf {
    virtual void add_cube(const position3 position, const std::string& label) = 0;
    virtual void add_arrow(const position3 from, const position3 to) = 0;
    virtual void run() = 0;
    virtual ~View_Intf() { }
};
