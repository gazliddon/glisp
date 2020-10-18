#ifndef EVAL_H_I8ZWFS1M
#define EVAL_H_I8ZWFS1M

#include "ast.h"
#include "reader.h"
#include <stack>

namespace ast {

    // Visitor to evaluate this expression
    struct Evaluator {
        env_t mEnv;

        std::stack<glisp::reader_reslult_t> mReaderContext;

        std::stack<size_t> mLines;

        val eval(glisp::reader_reslult_t _r) {
            mReaderContext.push(_r);
            val ret;
            auto const& ctx = mReaderContext.top();

            for (auto const& v : _r.mAst.mForms) {
                /* mLines.push(ctx.getLine(v)); */
                ret = eval(v);
                /* mLines.pop(); */
            }

            mReaderContext.pop();
            return ret;
        }

        val eval(ast::program const& _prog) {
            val ret;
            for (auto const& v : _prog.mForms) {
                /* mLines.push(_r.getLine(v)); */
                ret = eval(v);
                /* mLines.pop(); */
            }
            return ret;
        }

        val const& eval(val const& _v) const;
        val eval(val const& _v);

        val operator()(define const& _v);
        val operator()(bool const&);
        val operator()(ast::symbol const& _v);
        val operator()(ast::keyword const& _keyword);
        val operator()(std::string const& _v);
        val operator()(ast::hint const& _hint);
        val operator()(ast::nil const&);
        val operator()(double _v);
        val operator()(char _v);
        val operator()(ast::set const& _set);
        val operator()(ast::vector const& _vector);
        val operator()(ast::map const& _map);
        val operator()(ast::meta const& _value);
        val operator()(ast::lambda const& _lambda);
        val operator()(ast::native_function const& _lambda);
        val operator()(ast::sexp const& _sexp);
        val operator()(ast::program const& _program);
        val operator()(ast::macro const& _macro);

        val operator()(ast::arg const& _macro) {
            return ast::val();
        }

        val operator()(ast::let const& _let);

        void testEval();

        template <typename T>
        void eval_collection_in_place(T& _col) {
            auto b         = _col.begin();
            auto e         = _col.end();
            bool all_atoms = true;
        }

        void set(std::string const & _name, ast::val const & _v) {
            mEnv = mEnv.set(_name, _v);
        }

        void add_native_function(std::string const& _name,
            std::function<val(Evaluator& e, std::vector<val> const&)>&& _func,
            int _nargs) {

            native_function x {
                .mFunc      = std::move(_func),
                .mNumOfArgs = _nargs,
            };

            set(_name, val(x));
        }

        template <typename T, typename R = T>
        void add_twin_op(std::string const& _name,
            std::function<R(T const&, T const&)>&& _func) {

            auto f = std::move(_func);

            auto func = [f](Evaluator&, std::vector<val> const& _args) {
                assert(_args.size() == 2);
                auto a0 = _args[0].get_val<T>();
                auto a1 = _args[1].get_val<T>();
                return ast::val(f(*a0, *a1));
            };

            add_native_function(_name, func, 2);
        }

        std::stack<ast::val> mCallStack;
    };
}

#endif /* end of include guard: EVAL_H_I8ZWFS1M */
