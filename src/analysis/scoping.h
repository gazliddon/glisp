#ifndef SCOPING_H_ES3E7MZK
#define SCOPING_H_ES3E7MZK

#include "../ast.h"
#include "../cscoper.h"

// Traverse AST and lexically scope 
// all symbols correctly

namespace analysis {
    glisp::cScoper lexicallyScope(glisp::cScoper const & _scoper, ast::val & _ast);
}

#endif /* end of include guard: SCOPING_H_ES3E7MZK */
