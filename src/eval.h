#ifndef EVAL_H_I8ZWFS1M
#define EVAL_H_I8ZWFS1M

#include "demangle.h"
#include "env.h"
#include "reader.h"
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

        resolved_symbol_t addSymbol(symbol const& _sym) {
            uint64_t id = mEnv.symbolToId(_sym.mName);
            return { id };
        }

        resolved_symbol_t addAndSetSymbol(symbol const& _sym, val const& _v) {
            auto r = addSymbol(_sym);
            setSymbol(r, _v);
            return r;
        }

        void setSymbol(resolved_symbol_t _sym, val const& _val) {
            mEnv.setSymbol(_sym.mSymbolId, _val);
        }

        val const* getSymbol(resolved_symbol_t _sym) const {
            return mEnv.getSymbolValue(_sym.mSymbolId);
        }

        val const * getSymbol(symbol const & _sym) {
            auto x = mEnv.getSymbolId(_sym.mName);
            return mEnv.getSymbolValue(x);
        }

    protected:
        c_env_t mEnv;
        std::stack<c_env_t> mEnvs;
    };

    // Visitor to evaluate this expression
    struct Evaluator : public boost::static_visitor<val> {
        val eval(glisp::reader_reslult_t _r);

        void replaceNode(ast::val const& _val);
        ast::val& currentNode();

        bool mCanRecur;
        std::stack<glisp::reader_reslult_t> mReaderContext;
        std::stack<size_t> mLines;

        template <typename T>
        val operator()(T& _val) {
            std::cout << "Can't eval!" << demangle(_val) << std::endl;
            assert(false);
        }

        template <typename T>
        val operator()(x3::forward_ast<T>& _val) {
            return (*this)(_val.get());
        }


        val eval(val& _v);

        val operator()(ast::resolved_symbol_t& _v);
        val eval(ast::program& _prog);
        val operator()(define& _v);
        val operator()(ast::symbol& _v);
        val operator()(ast::set& _set);
        val operator()(ast::vector& _vector);
        val operator()(ast::map& _map);
        val operator()(ast::meta& _value);
        val operator()(ast::sexp& _sexp);
        val operator()(ast::program& _program);
        val operator()(ast::macro& _macro);
        val operator()(ast::let& _let);

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

        cEnv mEnvironment;
    };
}

#endif /* end of include guard: EVAL_H_I8ZWFS1M */
