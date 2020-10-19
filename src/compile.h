#ifndef COMPILE_H_IEAGQOPX
#define COMPILE_H_IEAGQOPX

#include "ast.h"

namespace glisp {

    struct compiled_t {
    };

    class cCompiler {


        public:
            cCompiler();
            compiled_t compile(ast::program const & _ast);

        protected:
    };
}


#endif /* end of include guard: COMPILE_H_IEAGQOPX */
