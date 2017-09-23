#ifndef EMBEDDED_NAMESPACES_HPP
#define EMBEDDED_NAMESPACES_HPP

template<class T> bool function_bar() {
    T temp;
    return true;
}

template<> bool function_bar<bool>() {
    bool temp;
    return true;
}

template<> bool function_bar<int>() {
    int temp;
    return true;
}

#endif /* EMBEDDED_NAMESPACES_HPP */

