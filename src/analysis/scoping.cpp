#include "scoping.h"

namespace analysis {
    struct scope_analyzer_t : boost::static_visitor<void> {

        glisp::cScoper& mScopes;

        ast::symbol_t mSymLambda;
        ast::symbol_t mSymDefine;
        ast::symbol_t mSymQuote;
        uint64_t mUserId;

        ast::val* mpVal;

        scope_analyzer_t(glisp::cScoper& _scopes)
            : mScopes(_scopes)
            , mpVal(nullptr) {
            mSymLambda = *mScopes.resolveSymbol("fn");
            mSymDefine = *mScopes.resolveSymbol("def");
            mSymQuote  = *mScopes.resolveSymbol("quote");
            mUserId    = *mScopes.getScopeId("user");
        }

        void operator()(ast::symbol_t& _v) {
        }

        void operator()(ast::keyword& _keyword) {
        }

        void operator()(ast::set& _set) {
            visitSeq(ast::cIterator(_set));
        }

        void operator()(ast::vector& _func) {
            visitSeq(_func);
        }

        void visitSeq(ast::cIterator&& _it) {
            while (auto p = _it.next()) {
                (*this)(*p);
            }
        }

        void operator()(ast::map& _map) {
        }

        void operator()(ast::meta& _value) {
        }

        void operator()(ast::map_entry& _map_entry) {
        }

        void operator()(ast::sexp& _func) {
            auto it = ast::cIterator(_func);

            if (auto p = it.next()) {
                if (auto sym = p->get<ast::symbol_t>()) {
                    if (*sym == mSymDefine) {
                        onDefine(it);
                        return;
                    }

                    if (*sym == mSymLambda) {
                        onLambda(it);
                        return;
                    }

                    if (*sym == mSymLambda) {
                        onLambda(it);
                        return;
                    }
                }
            }
        }

        void onQuote(ast::cIterator& _it) {
        }

        void onLet(ast::cIterator& _it) {
            mScopes.pushGenScope("let");
            mScopes.pop();
        }

        void onDefine(ast::cIterator& _it) {
        }

        void onLambda(ast::cIterator& _it) {
            fmt::format("LAMBDA!\n");
            mScopes.pushGenScope("fn");

            if (auto vp = _it.next_of<ast::vector>()) {

                ast::cIterator it(*vp);

                it.iterate<ast::symbol_t>([this](ast::symbol_t const& _sym) {
                    if (auto name = mScopes.getSymbolName(_sym)) {
                        mScopes.registerSymbol(*name, true);
                    }
                });
            }

            mScopes.pop();
        }

        void operator()(ast::program& _program) {
            visitSeq(_program);
        }

        template <typename X>
        void operator()(ast::forward_ast<X> const& _val) const {
            return (*this)(_val.get());
        }

        template <typename X>
        void operator()(X& _val) {
        }

        void operator()(ast::val& _val) {
            fmt::format("VAL!\n");
            mpVal = &_val;
            boost::apply_visitor(*this, _val);
            mpVal = nullptr;
        }
    };

    glisp::cScoper lexicallyScope(
        glisp::cScoper const& _scoper, ast::val& _ast) {
            fmt::format("Lexically scoping\n");

        auto scoper = _scoper;

        scope_analyzer_t analyzer(scoper);

        analyzer(_ast);

        fmt::format("Lexically scoping complete\n");

        return scoper;

    }
}
