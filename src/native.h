#ifndef NATIVE_H_4CGJ8FRM
#define NATIVE_H_4CGJ8FRM

#include "eval.h"

namespace glisp {

    void add_natives(ast::Evaluator& evaluator);
    ast::val include(ast::Evaluator& evaluator, std::string const& _fileName);
    
    ast::val slurp_x(ast::Evaluator& _e, std::vector<ast::val> const& _args);
    ast::val read_fn_x(ast::Evaluator& _e, std::vector<ast::val> const& _args);
}

#endif /* end of include guard: NATIVE_H_4CGJ8FRM */
