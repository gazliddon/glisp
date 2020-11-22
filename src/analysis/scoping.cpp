#include "scoping.h"

namespace analysis {
    struct scope_analyzer_t {

        glisp::cScoper & mScopes;

        ast::symbol_t mSymLambda;
        ast::symbol_t mSymDefine;
        ast::symbol_t mSymQuote;
        uint64_t mUserId;

        scope_analyzer_t(glisp::cScoper & _scopes) : mScopes(_scopes) {
            mSymLambda = *mScopes.resolveSymbol("fn");
            mSymDefine = *mScopes.resolveSymbol("def");
            mSymQuote = *mScopes.resolveSymbol("quote");
            mUserId = *mScopes.getScopeId("user");
        }

        void operator()(ast::symbol_t & _v)  {
        }

        void operator()(ast::keyword & _keyword)  {
        }

        void operator()(ast::set & _set)  {
            visitSeq(_set);
        }

        void operator()(ast::vector & _func)  {
            visitSeq(_func);
        }

        void visitSeq(ast::seq_t & _seq) {
            auto it = _seq.iterator();
            while(auto p = it->next()) {
                (*this)(*p);
            }
        }

        void operator()(ast::map & _map)  {
        }

        void operator()(ast::meta & _value)  {
        }

        void operator()(ast::map_entry & _map_entry)  {
        }

        void operator()(ast::sexp & _func)  {
            auto it = _func.iterator();

            if (auto p = it->next()) {
                if (auto sym = p->get<ast::symbol_t>() ) {
                    if(*sym == mSymDefine) {
                        onDefine(*it);
                        return;
                    }

                    if (*sym == mSymLambda) {
                        onLambda(*it);
                        return;
                    }

                    if (*sym == mSymLambda) {
                        onLambda(*it);
                        return;
                    }
                }
            }
        }

        void onQuote(ast::iterator_base_t & _it) {
        }

        void onLet(ast::iterator_base_t & _it) {
            mScopes.pushGenScope("fn");
            mScopes.pop();
        }

        void onDefine(ast::iterator_base_t & _it) {
        }

        void onLambda(ast::iterator_base_t & _it) {
            mScopes.pushGenScope("fn");
            mScopes.pop();
        }

        void operator()(ast::program & _program)  {
            visitSeq(_program);
        }

        template <typename X>
        void operator()(ast::forward_ast<X> const& _val) const {
            return (*this)(_val.get());
        }

        template <typename X>
        void operator()(X & _val)  {
        }

        void operator()(ast::val & _val)  {
            boost::apply_visitor(*this, _val);
        }
    };

    glisp::cScoper lexicallyScope(
        glisp::cScoper const& _scoper, ast::val& _ast) {

        return _scoper;
    }
}
