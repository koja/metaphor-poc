#pragma once

#include <experimental/optional>

// TODO my own interface?
template<typename T>using Maybe = std::experimental::optional<T>;