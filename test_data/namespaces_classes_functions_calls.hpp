#ifndef EMBEDDED_NAMESPACES_AND_CLASSES_HPP
#define EMBEDDED_NAMESPACES_AND_CLASSES_HPP

namespace std {
    class string {};

    void to_string(int) {}
        
    int func() {
        class LOCAL_CLASS {
            class LOCAL_SUBCLASS {};
        };

        string a;

        to_string(45);

        return 42;
    }
}

int my_sweet_func1() {

    std::func();
    
    return 42;
}

#endif

