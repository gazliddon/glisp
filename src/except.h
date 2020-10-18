#ifndef EXCEPT_H_AHGXLW8W
#define EXCEPT_H_AHGXLW8W

#include <exception>
#include <string>

namespace glisp {

    class cEvalError : public std::exception {
    public:
        cEvalError(char const* _error)
            : mError(_error) {
        }

        virtual char const* what() const noexcept {
            return mError.c_str();
        }

    protected:
        std::string mError;
    };
}

#endif /* end of include guard: EXCEPT_H_AHGXLW8W */
