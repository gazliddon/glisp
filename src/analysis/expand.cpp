#include "expand.h"

#include "visitor.h"

namespace analysis {
    struct expand_analyzer_t : public anaylyzer_base_t {
        ast::symbol_t mSymLambda;
        ast::symbol_t mSymDefine;
        ast::symbol_t mSymQuote;
        ast::symbol_t mSymLet;

        expand_analyzer_t(ast::cContext& _ctx)
            : anaylyzer_base_t(_ctx) {
            auto& scopes = mContext.getScoper();

            mSymLambda = *scopes.resolveSymbol("fn");
            mSymDefine = *scopes.resolveSymbol("def");
            mSymQuote  = *scopes.resolveSymbol("quote");
            mSymLet    = *scopes.resolveSymbol("let");
        }

        void onDefine(ast::cIterator& _it) {
            fmt::print("Expanding define\n");
            ast::define d;

            if (auto sym = _it.next_as<ast::symbol_t>()) {
                d.mSym = *sym;
            } else {
                throw("Expected a symbol! in define");
            }

            if (auto v = _it.next()) {
                analyze(*v);
                d.mVal = *v;
            } else {
                d.mVal = ast::unbound();
            }

            if (_it.remaining() != 0) {
                throw("Spurious items in define");
            }

            *mAstStack.top() = d;
        }

        void onLambda(ast::cIterator& _it) {
            fmt::print("Expanding Lambda\n");
            ast::lambda l;

            l.mDocString = _it.next_if<std::string>();

            if (auto vp = _it.next_as<ast::vector>()) {

                ast::cIterator vit(*vp);

                if (!vit.allType<ast::symbol_t>()) {
                    throw("Found a non-symbol in arg pack");
                }

                l.mArgs          = ast::args { .mArgs = *vp };
                l.mBody.mForms   = _it.into();
                *mAstStack.top() = l;

                analyze(l.mBody);

            } else {
                throw("Expected a vector!");
            }
        }

        void onLet(ast::cIterator& _it) {
            fmt::print("Expanding let\n");
            ast::let l;

            if (auto vp = _it.next_as<ast::vector>()) {

                ast::cIterator vit(*vp);

                if (vit.size() % 1 == 1) {
                    throw("Let binding has uneven amount of items");
                }

                while(!vit.finished()) {
                    auto sym = vit.next_as<ast::symbol_t>();
                    if (!sym) {
                        throw("Expecting symbol");
                    }

                    auto exp = vit.next();
                    analyze(*exp);

                    l.mBindings.mBindings.push_back(*sym);
                    l.mBindings.mBindings.push_back(*exp);
                }

                l.mBody.mForms = _it.into();
                analyze(l.mBody);
                *mAstStack.top() = l;

            } else {
                throw("Expected a vector as first part of let");
            }
        }

        virtual void operator()(ast::sexp& _func) {
            auto& scopes = mContext.getScoper();

            auto it = ast::cIterator(_func);

            if (it.remaining() == 0) {
                return;
            }

            if (auto sym = it.first_as<ast::symbol_t>()) {
                if (auto name = scopes.getSymbolName(*sym)) {
                } else {
                    throw("NO SYM");
                }

                if (*sym == mSymDefine) {
                    it.drop();
                    onDefine(it);
                    return;
                }

                if (*sym == mSymLambda) {
                    it.drop();
                    onLambda(it);
                    return;
                }

                if (*sym == mSymLet) {
                    it.drop();
                    onLet(it);
                    return;
                }

                if (*sym == mSymQuote) {
                    return;
                }

                analyzeSeq(it);

            } else {
                fmt::print("Not a symbol: ");
                if (auto x = it.first()) {
                    fmt::print("{}\n", x->typeName());
                } else {
                    fmt::print("No type? !!!!!!!!!!!!!!!\n");
                    fmt::print("it size = {}\n", it.size());
                    fmt::print("remaings = {}\n", it.remaining());
                }
            }
        }
    };

    void expandAst(ast::cContext& _ctx, ast::val& _ast) {
        fmt::print("Expanding\n");
        expand_analyzer_t analyzer(_ctx);
        analyzer.analyze(_ast);
    }
}
