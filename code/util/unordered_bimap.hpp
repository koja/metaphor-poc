#pragma once

#include <unordered_map>

template<typename T1, typename T2>
class unordered_bimap {
    public:
        unordered_bimap() = default;
        unordered_bimap(const unordered_bimap&) = default;
        unordered_bimap(unordered_bimap&&) = default;
        unordered_bimap& operator=(const unordered_bimap&) = default;

        template<typename T1_init, typename T2_init>
            void emplace(T1_init&& _val1, T2_init&& _val2) {
                umap_1_to_2.emplace(_val1, _val2);
                umap_2_to_1.emplace(_val2, _val1);
            }

        const T2& at1(const T1& val1) const { return umap_1_to_2.at(val1); }
        const T1& at2(const T2& val2) const { return umap_2_to_1.at(val2); }

        const std::unordered_map<T1, T2>& get_1_to_2() const { return umap_1_to_2; }
        const std::unordered_map<T2, T1>& get_2_to_1() const { return umap_2_to_1; }

        std::size_t size() const { return umap_1_to_2.size(); }
        /*
            TODO in case of need implement rest of the interface:
                remove(), clear(), ctor
        */

    private:
        std::unordered_map<T1, T2> umap_1_to_2;
        std::unordered_map<T2, T1> umap_2_to_1;
};
