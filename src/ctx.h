#ifndef CTX_H_HVJ1UWHY
#define CTX_H_HVJ1UWHY

// Ancillary information for the AST
// contains symbol registry
// source database

#include "source.h"
#include "cscoper.h"
#include "ast.h"

namespace ast {

    class cContext {
        public:
            cContext();

            glisp::cScoper & getScoper() {
                return mScopes;
            }
            glisp::cScoper const & getScoper() const {
                return mScopes;
            }

            cSourceStore & getSourceStore() {
                return mSources;
            }
            cSourceStore const & getSourceStore() const {
                return mSources;
            }

        protected:
            cSourceStore mSources;
            glisp::cScoper mScopes;
    };

}

#endif /* end of include guard: CTX_H_HVJ1UWHY */
