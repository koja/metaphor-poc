#pragma once

#include "util/graph/include/bi_graph.hpp"
#include "call_graph.hpp"

#include <functional>
#include <string>
#include <unordered_map>

namespace clang {
    class FunctionDecl;
}

struct Call_Graph_Builder_Intf {
    virtual void add_function(const clang::FunctionDecl* const function) = 0;
    virtual void add_call(const clang::FunctionDecl* const callee, const clang::FunctionDecl* const caller) = 0;
    virtual Call_Graph build() const = 0;
    virtual ~Call_Graph_Builder_Intf() { }
};

class Call_Graph_Builder : public Call_Graph_Builder_Intf {
    public:
        // TODO might later need to distinguis between functions with same mangled name in different binaries
        virtual void add_function(const clang::FunctionDecl* const function) override;
        virtual void add_call(const clang::FunctionDecl* const callee, const clang::FunctionDecl* const caller) override;
        virtual Call_Graph build() const override;

    private:
        Bi_Graph calls_;
        typedef Bi_Graph::Id Vertex_Id;

        unordered_bimap<const clang::FunctionDecl*, Vertex_Id> function_vertex_iso_;
        unordered_bimap<Vertex_Id, std::string> function_mangled_name_iso_;
        std::unordered_map<Vertex_Id, std::string> function_name_;
        std::unordered_map<Vertex_Id, std::unordered_map<Vertex_Id, std::size_t>> call_count_;
};
