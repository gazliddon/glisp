#ifndef SCOPING_H_ES3E7MZK
#define SCOPING_H_ES3E7MZK

#include "../ast.h"
#include "../ctx.h"
#include "../citerator.h"

// Traverse AST and lexically scope 
// all symbols correctly

namespace analysis {
    void lexicallyScope(ast::cContext & _scoper, ast::val & _ast);
}

#endif /* end of include guard: SCOPING_H_ES3E7MZK */
