#ifndef EVAL_H_I8ZWFS1M
#define EVAL_H_I8ZWFS1M

#include "ast.h"
#include "env.h"

namespace ast {

    // Visitor to evaluate this expression
    struct Evaluator {
        env_t mEnv;

        val eval(program const& _v);

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
        val operator()(ast::list const& _list);
        val operator()(ast::vector const& _vector);
        val operator()(ast::map const& _map);
        val operator()(ast::meta const& _value);
        val operator()(ast::map_entry const& _map_entry);
        val operator()(ast::lambda const& _lambda);
        val operator()(ast::native_function const& _lambda);
        val operator()(ast::function const& _lambda);

        void testEval();

        template <typename T>
        void eval_collection_in_place(T& _col) {
            auto b         = _col.begin();
            auto e         = _col.end();
            bool all_atoms = true;
        }

        void add_native_function(std::string const& _name,
            std::function<val(env_t, std::vector<val> const&)> _func,
            int _nargs) {
            mEnv = ::ast::add_native_function(mEnv, _name, _func, _nargs);
        }

        template <typename T>
        void add_twin_op(std::string const& _name,
            std::function<T(T const&, T const&)>&& _func) {

            auto func = [&_func](env_t, std::vector<val> const& _args) {
                assert(_args.size() == 2);
                auto a0 = _args[0].get_val<T>();
                auto a1 = _args[1].get_val<T>();
                return ast::val(_func(*a0, *a1));
            };

            mEnv = ::ast::add_native_function(mEnv, _name, func, 2);
        }

    };
}

#endif /* end of include guard: EVAL_H_I8ZWFS1M */
