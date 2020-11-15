#ifndef EVAL_H_I8ZWFS1M
#define EVAL_H_I8ZWFS1M

#include "demangle.h"
#include "env.h"
#include "reader.h"
#include <immer/algorithm.hpp>
#include <immer/map_transient.hpp>
#include <stack>
#include <string_view>

namespace ast {
    struct symbol_hash {
        std::size_t operator()(ast::symbol const& s) const noexcept {
            auto id = s.mId & 0x0000ffffffffffff;
            auto scope = s.mScope << 48;
            return id | scope;
        }
    };

    struct cEnv {
    public:
        cEnv() {
        }

        void setSymbol(symbol _sym, val const& _val) {
            mImmerMap = mImmerMap.insert({ _sym, _val });
        }

        boost::optional<val const&> getSymbol(symbol _sym) const {
            auto it = mImmerMap.find(_sym);
            return as_opt(it);
        }

        void enumerate(
            std::function<void(ast::symbol, val const&)> _func) const {
            immer::for_each(
                mImmerMap, [_func](std::pair<symbol, val> const& p) {
                    _func(p.first, p.second);
                });
        }

    protected:
        static auto constexpr as_opt
            = [](val const* _val) -> boost::optional<val const&> {
            if (_val) {
                return *_val;
            } else {
                return {};
            }
        };

        immer::map<ast::symbol, ast::val, symbol_hash> mImmerMap;
    };

    // Visitor to evaluate this expression
    struct Evaluator : public boost::static_visitor<val> {

        val eval(val const& _v);
        val eval(glisp::cReader::reader_reslult_t& ast);

        val readAndEval(std::string const& _str);

        glisp::cReader::reader_reslult_t read(std::string const& _str);

        void replaceNode(ast::val const& _val);
        ast::val const& currentNode() const;

        bool mCanRecur;
        /* std::stack<glisp::reader_reslult_t> mReaderContext; */
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

        /* val eval(ast::program& _prog); */
        val operator()(ast::symbol const& _v);
        val operator()(ast::define const& _v);
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
            std::function<val(Evaluator& e, iterator_base_t&)>&& _func,
            int _nargs);

        template <typename T, typename R = T>
        void add_twin_op(std::string const& _name,
            std::function<R(T const&, T const&)>&& _func) {
            auto f = std::move(_func);

            auto func = [f](Evaluator&, iterator_base_t& _args) {
                if (_args.size() != 2) {
                    std::cout << "args size is " << _args.size() << std::endl;
                }
                assert(_args.size() == 2);
                auto& a0 = *_args.next()->get<T>();
                auto& a1 = *_args.next()->get<T>();
                return ast::val(f(a0, a1));
            };

            add_native_function(_name, func, 2);
        }

        std::string symbolToName(ast::symbol const& _sym) const;

        Evaluator();

        std::string to_string(
            ast::val const& _val, bool add_types = false) const;
        std::string to_type_string(ast::val const& _val) const;

        std::string to_string(iterator_base_t& _it,
            char const* intersperse = " ",
            bool add_types          = false) const;

        void enumerateBindings(
            std::function<void(std::string const&, ast::val const&)> _func)
            const;

        void enumerateBindings(
            std::function<void(ast::symbol const&, ast::val const&)> _func)
            const;

        val apply(iterator_base_t& _exp) {
            return apply(_exp, mEnvironment);
        }

        val apply(val const& _val, iterator_base_t& _it) {
            return apply(_val, _it, mEnvironment);
        }

        void setCurrentNamespace(std::string const& _name);

    protected:
        unsigned mCallDepth;

        val apply(val const& _val, iterator_base_t& _it, cEnv localEnv);
        val apply(iterator_base_t& _exp, cEnv localEnv);

        std::stack<ast::val> mCallStack;
        glisp::cScoper mAllSymbols;
        glisp::cReader mReader;
        cEnv mEnvironment;
        ast::cSymTab mSymTab;

        ast::symbol mSf_if;
        ast::symbol mSf_and;
        ast::symbol mSf_or;
        ast::symbol mSf_apply;
        ast::symbol mSf_quote;
        ast::symbol mSf_do;
        ast::symbol mSf_comment;
    };
}

#endif /* end of include guard: EVAL_H_I8ZWFS1M */
