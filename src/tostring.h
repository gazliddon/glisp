#ifndef TOSTRING_H_XQFVJLRC
#define TOSTRING_H_XQFVJLRC


#include "ast.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <strstream>

namespace glisp {
    std::string to_type_string(ast::val const & _val);
    std::string to_string(ast::val const & _val, bool _withTypes= false);
}


#endif /* end of include guard: TOSTRING_H_XQFVJLRC */
