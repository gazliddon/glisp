#ifndef EVAL_H_I8ZWFS1M
#define EVAL_H_I8ZWFS1M

#include "ast.h"
#include "reader.h"
#include "demangle.h"
#include <stack>

namespace ast {

    // Visitor to evaluate this expression
    struct Evaluator : public boost::static_visitor<val> {
        env_t mEnv;
        bool mCanRecur;
        std::stack<glisp::reader_reslult_t> mReaderContext;
        std::stack<size_t> mLines;

        template <typename T>
        val operator()(T const& _val) {
            std::cout << "Can't eval!" << demangle(_val) << std::endl;
            assert(false);
        }

        template <typename T>
        val operator()(x3::forward_ast<T> const& _val) {
            return (*this)(_val.get());
        }

        val eval(glisp::reader_reslult_t _r);
        val eval(ast::program const& _prog);

        val const& eval(val const& _v) const;

        val eval(val const& _v);

        val operator()(define const& _v);
        val operator()(ast::symbol const& _v);
        val operator()(ast::set const& _set);
        val operator()(ast::vector const& _vector);
        val operator()(ast::map const& _map);
        val operator()(ast::meta const& _value);
        val operator()(ast::sexp const& _sexp);
        val operator()(ast::program const& _program);
        val operator()(ast::macro const& _macro);
        val operator()(ast::let const& _let);

        void testEval();


        void set(std::string const& _name, ast::val const& _v);

        void add_native_function(std::string const& _name,
            std::function<val(Evaluator& e, std::vector<val> const&)>&& _func,
            int _nargs);

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
