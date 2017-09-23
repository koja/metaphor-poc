#ifndef EMBEDDED_NAMESPACES_AND_CLASSES_HPP
#define EMBEDDED_NAMESPACES_AND_CLASSES_HPP

namespace n {       
	namespace m {
                namespace a { class AAA {}; }
                namespace b { }
		namespace c { }
		namespace x {
                    template<class T>class template_CCC {};
		}
		namespace z {}
                class BBB {};
	}
	namespace q {}
};

namespace std {
    class string {
        string() {}
        int length() { return 42; }
        bool empty() { return false; }

        static bool is_empty(const std::string& _in) { return false; }
    };

    int func() {
        return 42;
    }
    
    void to_string(int) {}
}

template<int T>class template_LOCAL_CLASS {
    class LOCAL_SUBCLASS {};
};

namespace x {
    template<class T>class template_CCC {};

    int my_sweet_func1() {
        return 42;
    }
}

#endif

