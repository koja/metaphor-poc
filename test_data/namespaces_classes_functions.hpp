#ifndef EMBEDDED_NAMESPACES_AND_CLASSES_HPP
#define EMBEDDED_NAMESPACES_AND_CLASSES_HPP

namespace n {       
	namespace m {
                namespace a { class AAA {}; }
                namespace b { }
		namespace c { }
		namespace x {
                    namespace y { }
                    class CCC {};
		}
		namespace z {}
                class BBB {};
	}
	namespace q {}
};

namespace std {
    class string {};

    int func() {
        class LOCAL_CLASS {
            class LOCAL_SUBCLASS {};
        };

        return 42;
    }
    
    void to_string(int) {}
}

int my_sweet_func1() {
    return 42;
}

#endif

