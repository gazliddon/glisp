#ifndef NATIVE_H_4CGJ8FRM
#define NATIVE_H_4CGJ8FRM

#include "eval.h"

namespace glisp {

    void add_natives(ast::Evaluator& evaluator);
    void include(ast::Evaluator & evaluator, std::string const & _fileName);
}

#endif /* end of include guard: NATIVE_H_4CGJ8FRM */
