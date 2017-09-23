#pragma once

#include <array>
#include <cmath>

static bool is_almost_zero(const double a) {

    return -std::pow(10, -50) < a && a < std::pow(10, -50);
}

static std::size_t sgn(const double x) {
    return (x > 0) - (x < 0);
}

static double non_zero(const double a) {

    return
        is_almost_zero(a)
        ?   std::numeric_limits<double>::epsilon() * (a != 0 ? sgn(a) : 1)
        :   a;
}

static double get_determinant_2D(
    const
        std::array<
            std::array<double, 2>,
            2
        >& A
) {
    return A[0][0] * A[1][1] - A[0][1] * A[1][0];
}

static double get_determinant_3D(
    const
        std::array<
            std::array<double, 3>,
            3
        >& A
) {
    return
            A[0][0] * A[1][1] * A[2][2]
        +   A[0][1] * A[1][2] * A[2][0]
        +   A[0][2] * A[1][0] * A[2][1]
        -   A[0][2] * A[1][1] * A[2][0]
        -   A[0][1] * A[1][0] * A[2][2]
        -   A[0][0] * A[1][2] * A[2][1]
        ;
}

/** Solves the pair of linear equations
 * Ax + By + C = 0
 * Dx + Ey + F = 0.
 * @returns {x, y} or {0, 0} in case determinant is close to zero.
 */
static std::array<double, 2> solve_2D_linear_equation(
    const double A, const double B, const double C,
    const double D, const double E, const double F
) {
    // TODO prepsat na get_determinant_2D())
    const double determinant = A*E - D*B;
    if( is_almost_zero(determinant) ){
        return std::array<double, 2>{0.0, 0.0};
    } else {
        // explicit Cramer rule for 2x2 system
        return std::array<double, 2>{
            (C * E - B * F)/determinant,
            (A * F - C * D)/determinant
        };
    }
}

/** Solves 3D linear equation by Cramer rule
 * @returns {x, y, z} or {0, 0, 0} in case determinant is close to zero.
 */
static std::array<double, 3> solve_3D_linear_equation(
    const
        std::array<
            std::array<double, 3>,
            3
        >& matrix,
    const std::array<double, 3>& right_side
) {
    const double D = get_determinant_3D(matrix);
    if( is_almost_zero(D) ) { return std::array<double, 3>{0.0, 0.0, 0.0}; }

    auto set_column_to = [](auto& M, const auto& v, const int c) {
        for(int i = 0; i < 3; ++i) {
            M[i][c] = v[i];
        }
    };

    auto M_x = matrix;
    set_column_to(M_x, right_side, 0);
    auto M_y = matrix;
    set_column_to(M_y, right_side, 1);
    auto M_z = matrix;
    set_column_to(M_z, right_side, 2);

    return std::array<double, 3>{
        get_determinant_3D(M_x) / D,
        get_determinant_3D(M_y) / D,
        get_determinant_3D(M_z) / D
    };
}