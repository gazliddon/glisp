#ifndef TOSTRING_H_XQFVJLRC
#define TOSTRING_H_XQFVJLRC


#include "ast.h"
#include "eval.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <strstream>
#include "citerator.h"

namespace glisp {
    std::string to_type_string(ast::val const & _val);
    std::string to_string(ast::Evaluator const & _eval, ast::val const & _val, bool _withTypes= false);
    std::string to_string(ast::Evaluator const & _eval,
        ast::cIterator & _it, char const* intersperse, bool _withTypes = false);
}


#endif /* end of include guard: TOSTRING_H_XQFVJLRC */
