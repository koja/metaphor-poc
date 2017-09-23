#pragma once

#include <string>

template<typename Tcontainer, typename TtransformToString>
inline std::string join(const Tcontainer& data, TtransformToString trans, const std::string& delim) {
    if( data.begin() == data.end() ) { return ""; }
    std::string result = trans( *data.begin() );
    for(auto it = ++(data.begin()); it != data.end(); ++it) {
        result += delim + trans(*it);
    }

    return result;
}

inline auto create_identity() -> auto {
    return [](const auto& x) { return x; };
}

template<typename TContainer>
inline TContainer pop_back(const TContainer& in, const size_t n) {
    if( n > in.size() ) { throw "container size smaller then n"; /* TODO */ }

    return TContainer( in.begin(), in.cend() - n );
}

template<typename TContainer>
void merge_to(TContainer& target, const TContainer& src) {

    target.insert( src.cbegin(), src.cend() );
}
