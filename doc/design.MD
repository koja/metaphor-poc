# code directory layout

- apps/
    final applications - binaries or libraries
- module/
    modules implementing specific high-level functionality (i. e. gui, graph layout algorithms)
- util/
    low-level functionality (i. e. functional tools for C++, low-level graph implementation)

Each module or util has:

- include/
    public interface (header files exporting functionality/data)
- src/
    implementation

# project design

## dependencies
Goal: Try to minimize dependencies especially between different modules but also between code in general. The goal is to be able to hot-swap implementation of specific part of code and don't have to make changes in other parts of code behind the interface.
Solution: Use abstract base classes for implementing interfaces.

### OPEN_ISSUES
- Currently the central entity is `Call_Graph` class. To have interface contract as small as possible it is implementing multiple interfaces - one for each specific client module (e. g. `Call_Graph_Intf_for_View`). This would lead to combinatorial explision if unchecked but currently helps to minimize dependencies. Will be revised later and might be changed.

# C++ usage

## type invariants [type_safety]
Each class MUST maintain it's invariants and existing instance MUST represent valid object unless there is a very good reason not to.

- Use constructors to establish valid instance and throw exceptions if this is not possible.
- Don't let class interface enable changes leading to invalid instance.

## conversions
Make every constructor (no matter how many parameters) `explicit` with the exception of only those that should be implicit and copy/move constructors.

### OPEN_ISSUES
Check that this is actually happening.

## strong typedefs [type_safety]
Don't use primitive (low-level) types in interfaces without a really good reason. Use rather one of these:

- custom class
- integer-based enum class
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0138r2.pdf

## exceptions
Using exceptions is the idiomatic C++ way of error handling.

## #pragma once
Albeit being a non-standard feature all relevant compilers implement it. Use `#pragma` once in favor of traditional `#include` guards.

## const correctness
Anything that could or should be const MUST be so.
### OPEN_ISSUES
- Research potential issues with some STL containers - e. g. `std::map<const int>`.
    - libstdc++ has obvious issues with e. g. std::map<const std::string, const std::set<const std::string>>. Why?
- Research if actually creating lambda and returning tmp object (hopefully moving not copying) to establish const instance is wasting resources or not.

>     const int a = [](){
>             int tmp = 0;
>             foo(tmp);
>             return tmp;
>     }();
