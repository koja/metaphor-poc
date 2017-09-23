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
}

#endif

