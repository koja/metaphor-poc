#ifndef DOUBLE3_60456364054
#define DOUBLE3_60456364054

#include "util/debug_print.hpp"

#include <array>
#include <string>

struct double3 {
    explicit double3(const double _x, const double _y, const double _z)
    :
        x(_x), y(_y), z(_z)
    { }
    
    double3(const double3& rhs)
    :
        x(rhs.x), y(rhs.y), z(rhs.z)
    { }
    
    double x;
    double y;
    double z;
};

struct position3 {
    explicit position3(const double _x, const double _y, const double _z)
    :
        x(_x), y(_y), z(_z)
    { }

    double x;
    double y;
    double z;
};

struct NegativeSize {};

class size3 {
    public:
        const double& x;
        const double& y;
        const double& z;

        size3& set_x(const double _x) {
            if(_x < 0) { throw NegativeSize{}; }
            x_ = _x;
            return *this;
        }

        size3& set_y(const double _y) {
            if(_y < 0) { throw NegativeSize{}; }
            y_ = _y;
            return *this;
        }

        size3& set_z(const double _z) {
            if(_z < 0) { throw NegativeSize{}; }
            z_ = _z;
            return *this;
        }

        explicit size3(const double _x, const double _y, const double _z)
        :
            x(x_), y(y_), z(z_)
        { 
            set_x(_x); set_y(_y); set_z(_z);
        }

        size3(const size3& rhs)
        :
            x(x_), y(y_), z(z_),
            x_(rhs.x_), y_(rhs.y_), z_(rhs.z_)
        { }

        void operator=(const size3& rhs) {
            x_ = rhs.x_; y_ = rhs.y_; z_ = rhs.z_;
        }
                
    private:
        double x_;
        double y_;
        double z_;
};

struct box3 {
    explicit box3(
        const position3& _center,
        const size3& _size
    ) :
        center(_center),
        size(_size)
    { }


    position3 center;
    size3 size;
};

// TODO tohle upravit

inline position3 operator+(const position3& lhs, const position3& rhs) {
    return position3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline position3 operator-(const position3& lhs, const position3& rhs) {
    return position3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline bool operator==(const position3& lhs, const position3& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}


inline size3 operator+(const size3& lhs, const size3& rhs) {
    return size3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline size3 operator-(const size3& lhs, const size3& rhs) {
    return size3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline bool operator==(const size3& lhs, const size3& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}


inline position3 operator+(const position3& lhs, const size3& rhs) {
    return position3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline position3 operator-(const position3& lhs, const size3& rhs) {
    return position3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}


inline size3 operator*(const size3& lhs, const double rhs) {
    return size3{lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}

inline position3 operator*(const position3& lhs, const double rhs) {
    return position3{lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}


inline double3 operator+(const double3& lhs, const double3& rhs) {
    return double3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline double3 operator-(const double3& lhs, const double3& rhs) {
    return double3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline double3 operator*(const double3& lhs, const double rhs) {
    return double3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

inline box3 get_bounding_box(const box3& a, const box3& b) {

    const double max_x = std::max( a.center.x + a.size.x * 0.5, b.center.x + b.size.x * 0.5 );
    const double max_y = std::max( a.center.y + a.size.y * 0.5, b.center.y + b.size.y * 0.5 );
    const double max_z = std::max( a.center.z + a.size.z * 0.5, b.center.z + b.size.z * 0.5 );

    const double min_x = std::min( a.center.x - a.size.x * 0.5, b.center.x - b.size.x * 0.5 );
    const double min_y = std::min( a.center.y - a.size.y * 0.5, b.center.y - b.size.y * 0.5 );
    const double min_z = std::min( a.center.z - a.size.z * 0.5, b.center.z - b.size.z * 0.5 );
    
    return 
        box3(
            position3(
                (max_x + min_x) * 0.5,
                (max_y + min_y) * 0.5,
                (max_z + min_z) * 0.5
            ),
            size3(
                (max_x - min_x),
                (max_y - min_y),
                (max_z - min_z)
            )
        );
}

inline position3 get_max(const box3& a) {
    return a.center + a.size * 0.5;
}

inline position3 get_min(const box3& a) {
    return a.center - a.size * 0.5;
}

template<> struct Debug_String<position3> {
    static std::string get(const position3& pos) {
        return "{ " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + " }";
    }
};

template<> struct Debug_String<size3> {
    static std::string get(const size3& s) {
        return "{ " + std::to_string(s.x) + ", " + std::to_string(s.y) + ", " + std::to_string(s.z) + " }";
    }
};

#endif
