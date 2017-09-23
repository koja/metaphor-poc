#ifndef EMBEDDED_NAMESPACES_HPP
#define EMBEDDED_NAMESPACES_HPP

struct A {
    A() = default;
    A(int) {}
    A(bool&) {}
    ~A(){}
};

bool function_bar() {
    A a;
    
    A b(5);

    return true;
}

#endif /* EMBEDDED_NAMESPACES_HPP */

