#ifndef EXCEPT_H_AHGXLW8W
#define EXCEPT_H_AHGXLW8W

#include <exception>
#include <string>
#include "ast.h"

namespace glisp {

    class cBaseException : public std::exception {
        public:
            cBaseException() {

            }
        virtual char const* where() const throw() = 0;
    };

    class cEvalError : public cBaseException {
    public:
        cEvalError(char const* _error)
            : mError(_error) {
        }

        virtual char const* what() const throw() {
            return mError.c_str();
        }

        virtual char const* where() const throw()  {
            return "NO IDEA";
        }
        

    protected:
        std::string mError;
    };
}

#endif /* end of include guard: EXCEPT_H_AHGXLW8W */
