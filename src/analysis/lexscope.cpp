#include "lexscope.h"
#include "../tostring.h"
#include "visitor.h"

#include <boost/mp11/mpl.hpp>
namespace analysis {

    using namespace boost::mp11;

    using lambda_type
        = mp_list<ast::symbol_t, boost::optional<std::string>, ast::vector>;

    template <typename... A>
    boost::optional<std::tuple<A&...>> getSequence(ast::cIterator& _it) {
        std::tuple<boost::optional<A&>...> x = { _it.next_as<A...>() };

        return {};
    }

    struct scope_analyzer_t : public anaylyzer_base_t {
        ast::symbol_t mSymLambda;
        ast::symbol_t mSymDefine;
        ast::symbol_t mSymQuote;
        ast::symbol_t mSymLet;

        scope_analyzer_t(ast::cContext& _ctx)
            : anaylyzer_base_t(_ctx) {
            auto& scopes = mContext.getScoper();

            mSymLambda = *scopes.resolveSymbol("fn");
            mSymDefine = *scopes.resolveSymbol("def");
            mSymQuote  = *scopes.resolveSymbol("quote");
            mSymLet    = *scopes.resolveSymbol("let");
        }

        virtual void operator()(ast::let& _val) {
            auto& scopes = mContext.getScoper();
        }

        virtual void operator()(ast::define& _val) {
            auto& scopes = mContext.getScoper();
        }

        virtual void operator()(ast::lambda& _val) {
            auto& scopes = mContext.getScoper();
            scopes.pushGenScope("fn");

            ast::cIterator argsIt(_val.mArgs.mArgs);

            argsIt.iterate<ast::symbol_t>([&scopes](ast::symbol_t& _sym) {
                if (auto name = scopes.getSymbolName(_sym)) {
                    fmt::print("Creating a new symbol", *name);
                    auto newSym = scopes.registerSymbol(*name);
                    _sym        = *newSym;
                }
            });

            analyze(_val.mBody);

            scopes.pop();
        }

        void operator()(ast::symbol_t& _sym) {
            auto& scopes = mContext.getScoper();

            if (auto name = scopes.getSymbolName(_sym)) {
                fmt::print("Symbol name is {}!!!\n", *name);
                auto newSym = scopes.resolveSymbol(*name);
                _sym        = *newSym;
            }
        }
    };

    void lexicallyScope(ast::cContext& _ctx, ast::val& _ast) {
        scope_analyzer_t analyzer(_ctx);
        analyzer.analyze(_ast);
    }
}
