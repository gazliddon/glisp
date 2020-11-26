#include "lexscope.h"

namespace analysis {
    struct scope_analyzer_t : boost::static_visitor<void> {

        ast::cContext & mContext;


        ast::symbol_t mSymLambda;
        ast::symbol_t mSymDefine;
        ast::symbol_t mSymQuote;
        uint64_t mUserId;

        ast::val* mpVal;

        scope_analyzer_t(ast::cContext & _ctx)
            : mContext(_ctx)
            , mpVal(nullptr) {
                auto & scopes = mContext.getScoper();

            mSymLambda = *scopes.resolveSymbol("fn");
            mSymDefine = *scopes.resolveSymbol("def");
            mSymQuote  = *scopes.resolveSymbol("quote");
            mUserId    = *scopes.getScopeId("user");
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
            auto & scopes = mContext.getScoper();

            fmt::print("SEXP!\n");

            auto it = ast::cIterator(_func);

            if (auto sym = it.next_as<ast::symbol_t>()) {

                fmt::print("Got a symbol of {}", *scopes.getSymbolName(*sym));

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
            } else {

                fmt::print("Not a symbol!");
            }
        }

        void onQuote(ast::cIterator& _it) {
        }

        void onLet(ast::cIterator& _it) {
            auto & scopes = mContext.getScoper();
            scopes.pushGenScope("let");
            scopes.pop();
        }

        void onDefine(ast::cIterator& _it) {
        }

        void onLambda(ast::cIterator& _it) {
            auto & scopes = mContext.getScoper();
            fmt::print("LAMBDA!\n");

            scopes.pushGenScope("fn");

            if (auto doc = _it.first_as<std::string>()) {
                _it.drop();
            }

            if (auto vp = _it.next_as<ast::vector>()) {

                fmt::print("Got a vector!\n");

                ast::cIterator it(*vp);

                it.iterate<ast::symbol_t>([&scopes](ast::symbol_t& _sym) {
                    fmt::print("Iterating symbol!\n");
                    if (auto name = scopes.getSymbolName(_sym)) {
                        fmt::print("Symbol name is {}!!!", *name);
                        auto newSym = scopes.registerSymbol(*name, true);
                        _sym        = *newSym;
                    }
                });

                _it.iterate([this](auto& _val) { (*this)(_val); });
            }

            scopes.pop();
        }

        void operator()(ast::program& _program) {
            fmt::print("program!\n");
            visitSeq(_program);
        }

        template <typename X>
        void operator()(boost::spirit::x3::forward_ast<X>& _val) {
            return operator()(_val.get());
        }
        template <typename X>
        void operator()(X& _val) {
            fmt::print("Uknown {}", demangle(_val));
        }

        void operator()(ast::val& _val) {
            fmt::print("VAL!\n");
            mpVal = &_val;
            try {

            boost::apply_visitor(*this, _val);

            } catch(...) {

            };

            mpVal = nullptr;
        }
    };

    void lexicallyScope(
        ast::cContext & _ctx, ast::val& _ast) {
        fmt::print("Lexically scoping\n");

        scope_analyzer_t analyzer(_ctx);

        analyzer(_ast);

        fmt::print("Lexically scoping complete\n");
    }
}
