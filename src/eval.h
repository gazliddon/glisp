#ifndef EVAL_H_I8ZWFS1M
#define EVAL_H_I8ZWFS1M

#include "demangle.h"
#include "env.h"
#include "reader.h"
#include <immer/map_transient.hpp>
#include <stack>

namespace ast {

    struct c_env_t {
        uint64_t mNext;
        immer::map<uint64_t, val> mMap;
        immer::map<uint64_t, std::string> mRefToSym;
        immer::map<std::string, uint64_t> mStringToId;

        uint64_t symbolToId(std::string const& _sym) {
            if (auto p = mStringToId.find(_sym)) {
                return *p;
            } else {
                auto ret  = mNext;
                mRefToSym = mRefToSym.insert({ mNext++, _sym });
                return ret;
            }
        }

        void setSymbol(uint64_t _id, val const& _val) {
            mMap = mMap.insert({ _id, _val });
        }

        val const* getSymbolValue(uint64_t _sym) const {
            return mMap.find(_sym);
        }

        uint64_t getSymbolId(std::string const& _name) {
            if (auto p = mStringToId.find(_name)) {
                return *p;
            } else {
                return symbolToId(_name);
            }
        }
    };

    struct cEnv {
    public:
        cEnv() {
        }

        symbol addAndSetSymbol(symbol const& _sym, val const& _v) {
            setSymbol(_sym, _v);
            return _sym;
        }

        void setArgs(args const& _args) {
        }

        void setSymbol(symbol _sym, val const& _val) {
            mEnv.setSymbol(_sym.mId, _val);
        }

        val const* getSymbol(symbol _sym) const {
            return mEnv.getSymbolValue(_sym.mId);
        }

        void enumerate(std::function<void(uint64_t, val const&)> _func) const {
            for (auto const& p : mEnv.mMap) {
                _func(p.first, p.second);
            }
        }

    protected:
        c_env_t mEnv;
        std::stack<c_env_t> mEnvs;
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

        std::string symbolToString(ast::symbol const& _sym) const;

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

    protected:
        unsigned mCallDepth;

        val apply(val const& _val, iterator_base_t& _it, cEnv localEnv);
        val apply(iterator_base_t& _exp, cEnv localEnv);

        std::stack<ast::val> mCallStack;
        glisp::cReader mReader;
        cEnv mEnvironment;
        glisp::cReaderSymTab mSymTab;

        ast::symbol mSf_if;
        ast::symbol mSf_and;
        ast::symbol mSf_or;
        ast::symbol mSf_apply;
        ast::symbol mSf_define;
        ast::symbol mSf_quote;
        ast::symbol mSf_do;
        ast::symbol mSf_comment;
    };
}

#endif /* end of include guard: EVAL_H_I8ZWFS1M */
