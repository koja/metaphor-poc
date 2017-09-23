#pragma once

#include <cstdlib>

std::size_t get_random_in_0_max(const std::size_t max) {
    return
        static_cast<std::size_t>(
            static_cast<double>(std::rand()) * static_cast<double>(max)
            /
            static_cast<double>(RAND_MAX)
        );
}

double get_random_double_0_1() {
    return
        static_cast<double>(std::rand())
        /
        static_cast<double>(RAND_MAX);
}

/**
 * @return randomly -1 or 1
 */
int get_random_sign() {
    return get_random_double_0_1() > 0.5 ? 1 : -1;
}