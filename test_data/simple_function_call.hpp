#ifndef EMBEDDED_NAMESPACES_HPP
#define EMBEDDED_NAMESPACES_HPP

bool function_foo() {
    return true;
}

bool function_bar() {
    return function_foo();
}

#endif /* EMBEDDED_NAMESPACES_HPP */

