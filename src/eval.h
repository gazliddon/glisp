#ifndef EVAL_H_I8ZWFS1M
#define EVAL_H_I8ZWFS1M

#include "citerator.h"
#include "demangle.h"
#include "env.h"
#include "reader.h"
#include <immer/algorithm.hpp>
#include <immer/map_transient.hpp>
#include <stack>
#include <string_view>

namespace ast {
    struct symbol_hash {
        std::size_t operator()(ast::symbol_t const& s) const noexcept {
            auto id    = s.mId & 0x0000ffffffffffff;
            auto scope = s.mScope << 48;
            return id | scope;
        }
    };

    struct cEnv {
    public:
        cEnv() {
        }

        void setSymbol(symbol_t _sym, val const& _val) {
            mImmerMap = mImmerMap.insert({ _sym, _val });
        }

        boost::optional<val const&> getSymbol(symbol_t _sym) const {
            auto it = mImmerMap.find(_sym);
            return as_opt(it);
        }

        void enumerate(std::function<void(symbol_t, val const&)> _func) const {
            immer::for_each(
                mImmerMap, [_func](std::pair<symbol_t, val> const& p) {
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

        immer::map<symbol_t, ast::val, symbol_hash> mImmerMap;
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
        val operator()(T & _val) {
            std::cout << "Can't eval!" << demangle(_val) << std::endl;
            assert(false);
        }

        template <typename T>
        val operator()(x3::forward_ast<T> & _val) {
            return (*this)(_val.get());
        }

        /* val eval(ast::program& _prog); */
        val operator()(symbol_t & _v);
        val operator()(define & _v);
        val operator()(set & _set);
        val operator()(vector & _vector);
        val operator()(map & _map);
        val operator()(meta & _value);
        val operator()(sexp & _sexp);
        val operator()(program & _program);
        val operator()(macro & _macro);
        val operator()(let & _let);

        void testEval();

        void set(std::string const& _name, ast::val const& _v);

        void add_native_function(std::string const& _name,
            std::function<val(Evaluator& e, ast::cIterator&)>&& _func,
            int _nargs);

        template <typename T, typename R = T>
        void add_twin_op(std::string const& _name,
            std::function<R(T const&, T const&)>&& _func) {
            auto f = std::move(_func);

            auto func = [f](Evaluator&, cIterator& _args) {
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

        std::string symbolToName(symbol_t const& _sym) const;

        Evaluator();

        std::string to_string(
            ast::val const& _val, bool add_types = false) const;
        std::string to_type_string(ast::val const& _val) const;

        std::string to_string(ast::cIterator& _it,
            char const* intersperse = " ",
            bool add_types          = false) const;

        void enumerateBindings(
            std::function<void(std::string const&, ast::val const&)> _func)
            const;

        void enumerateBindings(
            std::function<void(symbol_t const&, ast::val const&)> _func) const;

        val apply(ast::cIterator& _exp) {
            return apply(_exp, mEnvironment);
        }

        val apply(val & _val, ast::cIterator& _it) {
            return apply(_val, _it, mEnvironment);
        }

        void setCurrentNamespace(std::string const& _name);

        glisp::cScoper const& getAllSymbols() const {
            return mAllSymbols;
        }

    protected:
        unsigned mCallDepth;

        val apply(val & _val, ast::cIterator& _it, cEnv localEnv);
        val apply(ast::cIterator& _exp, cEnv localEnv);

        std::stack<ast::val> mCallStack;
        glisp::cScoper mAllSymbols;
        glisp::cReader mReader;
        cEnv mEnvironment;
        ast::cSymRegistry mSymTab;

        symbol_t mSf_if;
        symbol_t mSf_and;
        symbol_t mSf_or;
        symbol_t mSf_apply;
        symbol_t mSf_quote;
        symbol_t mSf_do;
        symbol_t mSf_comment;
    };
}

#endif /* end of include guard: EVAL_H_I8ZWFS1M */
