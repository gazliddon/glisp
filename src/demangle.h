#ifndef DEMANGLE_H_DVXNJTDA
#define DEMANGLE_H_DVXNJTDA

#include <cxxabi.h>
#include <string>

template <typename T>
std::string demangle() {
    const size_t max_size = 20000;

    char buff[max_size];
    auto mangled = typeid(T).name();
    int stat     = 0;
    size_t len   = max_size;

    auto c = abi::__cxa_demangle(mangled, buff, &len, &stat);
    return c;
}

template <typename T>
std::string demangle(T const& _arg) {
    return demangle<decltype(_arg)>();
}

#endif /* end of include guard: DEMANGLE_H_DVXNJTDA */
