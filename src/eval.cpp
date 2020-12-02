#include "eval.h"
#include "citerator.h"
#include "except.h"
#include "native.h"
#include "tostring.h"

#include <iostream>
#include <spdlog/spdlog.h>

namespace ast {
    namespace x3 = boost::spirit::x3;
    using namespace std;

    val eval_next(Evaluator& _e, cIterator& _it) {
        val ret;

        auto nx = _it.next();

        if (nx) {
            ret = _e.eval(*nx);
        }

        return ret;
    }

    static std::vector<val> eval_seq_to_vec(
        Evaluator& _e, ast::cIterator const& _it) {

        std::vector<val> ret;

        ret.reserve(_it.remaining());

        _it.iterate([&ret, &_e](auto const& v) { ret.push_back(_e.eval(v)); });

        return ret;
    }

    static sexp eval_to_exp(Evaluator& _e, ast::cIterator& _ptr) {
        sexp ret;
        ret.mForms = eval_seq_to_vec(_e, _ptr);
        return ret;
    }

    static val eval_seq(Evaluator& _e, ast::cIterator const& _it) {
        val ret;
        _it.iterate([&ret, &_e](auto const& v) { ret = _e.eval(v); });
        return ret;
    }

    template <typename T>
    static val eval_rest(Evaluator& _e, T& _seq) {
        auto it   = ast::cIterator(_seq);
        auto rest = it.rest();
        return eval_seq(_e, rest);
    }

    val Evaluator::readAndEval(std::string const& _str) {
        auto ast = read(_str);
        return eval(ast);
    }

    glisp::cReader::reader_reslult_t Evaluator::read(std::string const& _str) {
        return mReader.read(_str);
    }

    void Evaluator::add_native_function(std::string const& _name,
        std::function<val(Evaluator& e, cIterator&)>&& _func,
        int _nargs) {
        auto & scopes = getAstContext().getScoper(); 

        native_function x;
        x.mFunc      = std::move(_func);
        x.mNumOfArgs = _nargs;
        auto sym = ast::symbol_t { *scopes.registerSymbol(_name, true) };
        mEnvironment.setSymbol(sym, val(x));
    }

    val Evaluator::eval(glisp::cReader::reader_reslult_t& ast) {
        auto ret = eval(ast.mAst);
        return ret;
    }

    void Evaluator::set(std::string const& _name, ast::val const& _v) {
        assert(false);
    }

    val Evaluator::operator()(ast::macro& _macro) {
        assert(false);
        auto macro_val = ast::val(_macro);
        /* set(_macro.mSym.mName, macro_val); */
        return macro_val;
    }

    val Evaluator::operator()(ast::let& _let) {

        auto oldEnv = mEnvironment;

        auto ret = val();

        for (auto& binding : _let.mBindings.mBindings) {
            auto p = binding.get<pair>();
            assert(p);
            auto sym = p->mFirst.get<symbol_t>();
            mEnvironment.setSymbol(*sym, p->mSecond);
        }

        ret = eval_seq(*this, _let.mBody);

        mEnvironment = oldEnv;

        return ret;
    }

    val Evaluator::operator()(define& _v) {
        auto ret = _v.mVal;

        ret = eval(_v.mVal);
        mEnvironment.setSymbol(_v.mSym, ret);

        return val(_v.mSym);
    }

    val Evaluator::operator()(ast::symbol_t& _v) {

        if (auto sym = mEnvironment.getSymbol(_v)) {
            return *sym;
        }

        auto error = fmt::format("Unable to resolve symbol {} : ({} {})",
            symbolToName(_v),
            _v.mScope,
            _v.mId);

        throw glisp::cEvalError(error);
    }

    val Evaluator::operator()(ast::vector& _vector) {
        auto args = _vector;

        for (auto& v : args.mForms) {
            if (!v.is_atom()) {
                v = eval(v);
            }
        }

        return val(args);
    }

    val Evaluator::operator()(ast::map& _map) {
        ast::map ret = _map;

        for (auto& p : ret.mHashMap) {
            p.mKey   = eval(p.mKey);
            p.mValue = eval(p.mValue);
        }

        return val(ret);
    }

    template <class I>
    ast::val get_first(I _begin, I _end) {
        if (_end - _begin > 0) {
            return *_begin;
        } else {
            return ast::val(ast::nil());
        }
    }

    ast::val Evaluator::operator()(ast::program& _program) {
        return eval_seq(*this, _program);
    }

    ast::val get_first(std::vector<ast::val> const& _args) {
        return get_first(_args.begin(), _args.end());
    }

    template <typename T>
    boost::optional<T&> as(boost::optional<val&> ptr) {
        if (ptr) {
            return ptr->get<T>();
        } else {
            return {};
        }
    }

    template <typename T>
    T const* as(val const* ptr) {
        if (ptr) {
            return ptr->get<T>();
        } else {
            return nullptr;
        }
    }

    void print(Evaluator& e, cIterator const& _it) {
        _it.iterate([&e](auto const& v) {
            std::cout << e.to_string(v, true) << std::endl;
        });
    }

    val Evaluator::apply(ast::cIterator& _exp, cEnv localEnv) {
        auto rest  = _exp.rest();
        auto first = _exp.first();
        return apply(*first, rest, localEnv);
    }

    val Evaluator::apply(val& _first, cIterator& _rest, cEnv localEnv) {

        // TBD FIXIT!

        /* if (!_first.is_callable()) { */
        /*     auto error */
        /*         = fmt::format("Unable to call {}", to_string(val(_first)));
         */
        /*     error += "\n" + to_string(val(_first)); */

        /*     error += fmt::format("\n callable {}", */
        /*         std::is_base_of<val::callable_t, native_function>::value); */
        /*     error += fmt::format("\n var type {}", _first.var.which()); */
        /*     error += fmt::format("\n var type name {}", _first.get_name());
         */

        /*     throw glisp::cEvalError(error); */
        /* } */

        if (auto l = _first.get<lambda>()) {

            auto syms_it = ast::cIterator(l->mArgs.mArgs);

            auto args_passed = _rest.size();
            auto args_needed = syms_it.size();

            assert(args_passed >= args_needed);

            auto oldEnv = mEnvironment;

            mCallDepth++;

            syms_it.iterate([&_rest, this](auto const& val) {
                auto sym = val.template get<symbol_t>();
                mEnvironment.setSymbol(*sym, *_rest.next());
            });

            // Now evaluate the body
            auto ret = (*this)(l->mBody);

            mEnvironment = oldEnv;

            mCallDepth--;

            to_string(ret);

            return ret;
        }

        if (auto l = _first.get<native_function>()) {
            auto args    = eval_to_exp(*this, _rest);
            auto args_it = ast::cIterator(args);

            mCallDepth++;

            auto ret = l->call(*this, args_it);

            mCallDepth--;

            return ret;
        }

        throw glisp::cEvalError("This is odd");
        assert(false);
    }

    val Evaluator::operator()(ast::sexp& _func) {

        // Empty list evaluates to nil
        if (_func.mForms.empty()) {
            return val();
        }

        auto sexp_it = ast::cIterator(_func);
        auto seq     = sexp_it.clone();

        if (auto symptr = as<symbol_t>(seq.next())) {
            auto sym = *symptr;

            if (sym == mSf_quote) {
                return val(sexp(seq));
            }

            if (sym == mSf_do) {
                return eval_seq(*this, seq);
            }

            if (sym == mSf_comment) {
                return val();
            }

            if (sym == mSf_if) {
                assert(seq.remaining() >= 2);
                val ret;
                auto pred     = seq.next();
                auto is_true  = seq.next();
                auto is_false = seq.next();

                assert(pred && is_true);

                if (eval(*pred).to_bool()) {
                    ret = eval(*is_true);
                } else {
                    if (is_false) {
                        ret = eval(*is_false);
                    }
                }

                return ret;
            }
        }

        // Okay!
        // It's not a special form if we're here so lets eval!

        auto evaled = eval_to_exp(*this, sexp_it);
        auto it     = cIterator(evaled);
        return apply(it, mEnvironment);
    }

    void Evaluator::replaceNode(ast::val const& _val) {
        assert(false);
    }

    ast::val const& Evaluator::currentNode() const {
        assert(false);
    }

    void Evaluator::enumerateBindings(
        std::function<void(ast::symbol_t const&, ast::val const&)> _func)
        const {
        mEnvironment.enumerate(
            [_func](auto _sym, ast::val const& _val) { _func(_sym, _val); });
    }

    void Evaluator::enumerateBindings(
        std::function<void(std::string const&, ast::val const&)> _func) const {

        enumerateBindings(
            [this, &_func](ast::symbol_t const& sym, ast::val const& _val) {
                _func(symbolToName(sym), _val);
            });
    }

    val Evaluator::eval(val const& _v) {
        using namespace ast;

        val ret;

        if (_v.is<symbol_t>())
            return boost::apply_visitor(*this, _v);

        if (_v.is_atom() || _v.is<lambda>())
            return _v;
        else
            return boost::apply_visitor(*this, _v);
    }

    std::string Evaluator::to_type_string(ast::val const& _val) const {
        return glisp::to_type_string(_val);
    }

    std::string Evaluator::to_string(
        ast::val const& _val, bool _add_types) const {
        return glisp::to_string(*this, _val, _add_types);
    }

    std::string Evaluator::to_string(
        ast::cIterator& _it, char const* intersperse, bool _add_types) const {
        return glisp::to_string(*this, _it, intersperse, _add_types);
    }

    std::string Evaluator::symbolToName(ast::symbol_t const& _sym) const {
        auto& scopes = mContext.getScoper();
        auto ret = scopes.getScopedSymbolName({ _sym.mScope, _sym.mId });

        if (!ret) {
            fmt::print("Cannot find sym name {}:{}\n", _sym.mScope, _sym.mId);
            scopes.dump();
        }

        assert(ret);
        return *ret;
    }

    std::string Evaluator::symbolToUnscopedName(ast::symbol_t const& _sym) const {
        auto& scopes = mContext.getScoper();
        auto ret = scopes.getSymbolName({ _sym.mScope, _sym.mId });

        if (!ret) {
            fmt::print("Cannot find sym name {}:{}\n", _sym.mScope, _sym.mId);
            scopes.dump();
        }

        assert(ret);
        return *ret;
    }


    void registerSymbols(
        glisp::cScoper& _scopes, std::initializer_list<char const*> _vars) {
        for (auto& e : _vars) {
            _scopes.registerSymbol(e);
        }
    }

    Evaluator::Evaluator()
        : mCallDepth(0)
        , mReader(mContext) {

        auto& scopes = mContext.getScoper();

        scopes.push("special");

        registerSymbols(
            scopes, { "let", "loop", "recur", "def", "fn", "throw" });

        mSf_if      = *scopes.registerSymbol("if");
        mSf_and     = *scopes.registerSymbol("and");
        mSf_or      = *scopes.registerSymbol("or");
        mSf_quote   = *scopes.registerSymbol("quote");
        mSf_do      = *scopes.registerSymbol("do");
        mSf_comment = *scopes.registerSymbol("comment");

        scopes.push("native");

        glisp::add_natives(*this);
    }

    void Evaluator::setCurrentNamespace(std::string const& _name) {
        auto& scopes = mContext.getScoper();
        auto id      = scopes.getOrRegisterScope(_name);
        scopes.push(id);
    }

}
