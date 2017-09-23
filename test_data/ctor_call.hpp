#ifndef EMBEDDED_NAMESPACES_HPP
#define EMBEDDED_NAMESPACES_HPP

class A {
    A() = default;
    A(int) {}
    A(bool&) {}
    ~A(){}
};

bool function_bar() {
    A a;
    
    A b(5);
}

#endif /* EMBEDDED_NAMESPACES_HPP */

