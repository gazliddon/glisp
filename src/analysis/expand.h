#ifndef EXPAND_H_2VW8NKPH
#define EXPAND_H_2VW8NKPH

// Take stream of minimal symbols 
// and transform into higher level
// constructs
// eg ast::sexp (if a b c) xforms to ast::if
// checks for remedial syntax errors

#include "../ast.h"
#include "../ctx.h"
#include "../citerator.h"

namespace analysis {
    void expandAst(ast::cContext& _ctx, ast::val& _ast);
}

#endif /* end of include guard: EXPAND_H_2VW8NKPH */
