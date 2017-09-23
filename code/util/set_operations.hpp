#pragma once

template<typename TValue, typename ... TOptionalValues>
inline bool is_equal_to_any(const TValue& lhs, const TValue& head_rhs, const TOptionalValues&... tail_rhs) {
    return lhs == head_rhs || is_equal_to_any(lhs, tail_rhs...);
}

template<typename TValue>
inline bool is_equal_to_any(const TValue& lhs, const TValue& rhs) {
    return lhs == rhs;
}
