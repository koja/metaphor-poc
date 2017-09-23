#ifndef DEBUG_PRINT_748637147431
#define DEBUG_PRINT_748637147431

#include <string>

template<typename T> struct Debug_String {
    static std::string get(const T& _in) { return std::to_string(_in); }
};

template<> struct Debug_String<std::string> {
    static std::string get(const std::string& _in) { return _in; }
};

template<typename T> std::string debug_string(const T& _in) {
    return Debug_String<T>::get(_in);
}

#endif