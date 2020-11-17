#ifndef NATIVE_H_4CGJ8FRM
#define NATIVE_H_4CGJ8FRM

#include "eval.h"

namespace glisp {

    void add_natives(ast::Evaluator& evaluator);
    ast::val include(ast::Evaluator& evaluator, std::string const& _fileName);
    
    ast::val slurp(ast::Evaluator& _e, ast::iterator_base_t& _args);
    ast::val read_fn(ast::Evaluator& _e, ast::iterator_base_t& _args);

}

#endif /* end of include guard: NATIVE_H_4CGJ8FRM */
