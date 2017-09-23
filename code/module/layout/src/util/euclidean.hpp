#pragma once

#include "../../include/double3.hpp"

#include "random.hpp"

#include <cmath>
#include <vector>

inline double get_E_dist(const position3& a, const position3& b) {
    const double dx = a.x - b.x;
    const double dy = a.y - b.y;
    const double dz = a.z - b.z;
    return 0.5 * std::sqrt(dx*dx + dy*dy + dz*dz);
}



// TODO use
struct first_order_derivation2 {
    double dx;  double dy;

    first_order_derivation2(
        const double _dx, const double _dy
    ) :
        dx(_dx),    dy(_dy)
    { }
};

// TODO use
// expecting symmetry dxdy = dydx
struct second_order_derivation2 {
    double dx2; double dy2;
    double dxdy;

    second_order_derivation2(
        const double _dx2, const double _dy2,
        const double _dxdy
    ) :
        dx2(_dx2),  dy2(_dy2),
        dxdy(_dxdy)
    { }
};



struct first_order_derivation3 {
    double dx;  double dy;  double dz;

    first_order_derivation3(
        const double _dx, const double _dy, const double _dz
    ) :
        dx(_dx),    dy(_dy),    dz(_dz)
    { }
};

// expecting symmetry dxdy = dydx
struct second_order_derivation3 {
    double dx2; double dy2; double dz2;
    double dxdy;double dydz;double dzdx;

    second_order_derivation3(
        const double _dx2, const double _dy2, const double _dz2,
        const double _dxdy,const double _dydz,const double _dzdx
    ) :
        dx2(_dx2),  dy2(_dy2),  dz2(_dz2),
        dxdy(_dxdy),dydz(_dydz),dzdx(_dzdx)
    { }
};
