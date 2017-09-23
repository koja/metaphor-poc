#pragma once

#include "util/graph/include/bi_graph.hpp"
#include "util/unordered_bimap.hpp"

#include <functional>
#include <set>


enum class Function_Id : std::size_t { };

struct Function {
    Function(
        const Function_Id _id,
        const std::string& _name,
        const std::string& _mangled_name
    ) :
        id(_id),
        name(_name),
        mangled_name(_mangled_name)
    { }

    const Function_Id id;
    const std::string name;
    const std::string mangled_name;
};

struct Call_Graph_Intf {

    virtual void for_each_function( std::function<void(const Function&)> ) const = 0;
    virtual void for_each_call( std::function<void(const Function& /*caller*/, const Function& /*callee*/)> ) const = 0;

    virtual Function get_function(const Function_Id) const = 0;

    virtual std::size_t function_count() const = 0;

    virtual ~Call_Graph_Intf() { }
};

class Call_Graph :
    public Call_Graph_Intf
{
    public:
        typedef Bi_Graph::Id Vertex_Id;

        Call_Graph(
            const Bi_Graph& _calls,
            const unordered_bimap<Function_Id, Vertex_Id>& _function_vertex_iso,
            const unordered_bimap<Vertex_Id, std::string>& _function_mangled_name_iso,
            const std::unordered_map<Vertex_Id, std::string>& _function_name,
            const std::unordered_map<Vertex_Id, std::unordered_map<Vertex_Id, std::size_t>>& _call_count
        );
        virtual void for_each_function( std::function<void(const Function&)> ) const override;
        virtual void for_each_call( std::function<void(const Function&, const Function&)> ) const override;
        virtual Function get_function(const Function_Id) const override;
        virtual std::size_t function_count() const override;

    private:
        Bi_Graph calls_;

        unordered_bimap<Function_Id, Vertex_Id> function_vertex_iso_;
        unordered_bimap<Vertex_Id, std::string> function_mangled_name_iso_;
        std::unordered_map<Vertex_Id, std::string> function_name_;
        std::unordered_map<Vertex_Id, std::unordered_map<Vertex_Id, std::size_t>> call_count_;
};
