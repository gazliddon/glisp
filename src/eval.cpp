#include "eval.h"
#include "except.h"
#include "native.h"
#include "tostring.h"

#include <iostream>
#include <spdlog/spdlog.h>

namespace ast {
    namespace x3 = boost::spirit::x3;
    using namespace std;

    val eval_next(Evaluator& _e, iterator_base_t& _it) {
        val ret;

        auto nx = _it.next();

        if (nx) {
            ret = _e.eval(*nx);
        }

        return ret;
    }

    static std::vector<val> eval_seq_to_vec(
        Evaluator& _e, iterator_base_t& _it) {
        std::vector<val> ret;
        ret.reserve(_it.remaining());
        while (auto v = _it.next()) {
            ret.push_back(_e.eval(*v));
        }
        return ret;
    }

    static sexp eval_to_exp(Evaluator& _e, iterator_base_t& _ptr) {
        sexp ret;
        ret.mForms = eval_seq_to_vec(_e, _ptr);
        return ret;
    }

    static val eval_seq(Evaluator& _e, std::unique_ptr<iterator_base_t>& _it) {
        val ret;

        while (auto v = _it->next()) {
            ret = _e.eval(*v);
        }
        return ret;
    }

    template <typename T>
    static val eval_seq(Evaluator& _e, T& _seq) {
        auto it = _seq.iterator();
        return eval_seq(_e, it);
    }
    template <typename T>
    static val eval_rest(Evaluator& _e, T& _seq) {
        auto it = _seq.iterator();
        return eval_seq(_e, it->rest());
    }

    template <typename T>
    void eval_seq_inplace(Evaluator& _e, T& _seq) {
        auto it = _seq.iterator();
        while (auto v = it->next()) {
            *v = _e.eval(*v);
        }
    }

    val Evaluator::readAndEval(std::string const& _str) {
        auto ast = read(_str);
        return eval(ast);
    }

    glisp::cReader::reader_reslult_t Evaluator::read(std::string const& _str) {
        return mReader.read(_str);
    }

    void Evaluator::add_native_function(std::string const& _name,
        std::function<val(Evaluator& e, iterator_base_t&)>&& _func,
        int _nargs) {

        native_function x;
        x.mFunc      = std::move(_func);
        x.mNumOfArgs = _nargs;
        auto sym  = ast::symbol{mAllSymbols.registerDefaultSymbol(_name)};
        mEnvironment.addAndSetSymbol( sym , val(x));
    }

    val Evaluator::eval(glisp::cReader::reader_reslult_t& ast) {
        auto ret = eval(ast.mAst);
        return ret;
    }

    void Evaluator::set(std::string const& _name, ast::val const& _v) {
        assert(false);
    }

    val Evaluator::operator()(ast::macro const& _macro) {
        assert(false);
        auto macro_val = ast::val(_macro);
        /* set(_macro.mSym.mName, macro_val); */
        return macro_val;
    }

    val Evaluator::operator()(ast::let const& _let) {

        auto oldEnv = mEnvironment;

        auto ret = val();

        for (auto& binding : _let.mBindings.mBindings) {
            /* set(arg.mSymbol.mName, eval(arg.mVal)); */
        }

        ret = eval_seq(*this, _let.mBody);

        mEnvironment = oldEnv;

        return ret;
    }

    val Evaluator::operator()(define const& _v) {
        auto ret = _v.mVal;

        ret = eval(_v.mVal);
        mEnvironment.addAndSetSymbol(_v.mSym, ret);

        return val(_v.mSym);
    }

    val Evaluator::operator()(ast::symbol const& _v) {
        fmt::print("Sym to be evaled is {} {}\n", _v.mScope, _v.mId);
        auto val_p = mEnvironment.getSymbol(_v);

        if (!val_p) {
            auto error = fmt::format(
                "Unable to resolve symbol {}", symbolToName(_v));
            throw glisp::cEvalError(error);
        }

        return *val_p;
    }

    val Evaluator::operator()(ast::vector const& _vector) {
        auto args = _vector;

        for (auto& v : args.mForms) {
            if (!v.is_atom()) {
                v = eval(v);
            }
        }

        return val(args);
    }

    val Evaluator::operator()(ast::map const& _map) {
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

    template <class I>
    std::vector<ast::val> get_rest(I _begin, I _end) {
        std::vector<ast::val> ret;
        if (_end - _begin > 1) {
            _begin++;
            while (_begin != _end) {
                ret.push_back(*_begin);
                _begin++;
            }
        }
        return ret;
    }

    ast::val Evaluator::operator()(ast::program const& _program) {
        return eval_seq(*this, _program);
    }

    std::vector<ast::val> get_rest(std::vector<ast::val> const& _args) {
        return get_rest(_args.begin(), _args.end());
    }

    ast::val get_first(std::vector<ast::val> const& _args) {
        return get_first(_args.begin(), _args.end());
    }

    template <typename T>
    boost::optional<T const &> as(boost::optional<val const &> ptr) {
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

    void print(Evaluator& e, std::unique_ptr<iterator_base_t> const& _it) {
        auto it = _it->clone();
        while (auto p = it->next()) {
            std::cout << e.to_string(*p, true) << std::endl;
        }
    }

    val Evaluator::apply(iterator_base_t& _exp, cEnv localEnv) {
        return apply(*_exp.first(), *_exp.rest().get(), localEnv);
    }

    val Evaluator::apply(
        val const& _first, iterator_base_t& _rest, cEnv localEnv) {

        if (!_first.is_callable()) {
            auto error
                = fmt::format("Unable to call {}", to_string(val(_first)));
            error += "\n" + to_string(val(_first));

            error += fmt::format("\n callable {}", std::is_base_of<val::callable_t, native_function>::value);
            error += fmt::format("\n var type {}", _first.var.which());
            error += fmt::format("\n var type name {}", _first.get_name());

            throw glisp::cEvalError(error);
        }

        if (auto l = _first.get<lambda>()) {
            auto syms_it = l->mArgs.mArgs.iterator();

            auto args_passed = _rest.size();
            auto args_needed = syms_it->size();

            assert(args_passed >= args_needed);

            auto oldEnv = mEnvironment;

            mCallDepth++;

            while(auto sym_p = syms_it->next()) {
                auto sym = sym_p->get<symbol>();
                assert(sym);
                mEnvironment.addAndSetSymbol(*sym, *_rest.next());
            }

            // Now evaluate the body
            auto ret = (*this)(l->mBody);

            mEnvironment = oldEnv;

            mCallDepth--;

            to_string(ret);

            return ret;
        }

        if (auto l = _first.get<native_function>()) {
            auto args    = eval_to_exp(*this, _rest);
            auto args_it = args.iterator();

            mCallDepth++;

            auto ret = l->call(*this, *args_it.get());

            mCallDepth--;

            return ret;
        }

        throw glisp::cEvalError("This is odd");
        assert(false);
    }

    val Evaluator::operator()(ast::sexp const& _func) {

        // Empty list evaluates to nil
        if (_func.mForms.empty()) {
            return val();
        }

        auto sexp_it = _func.iterator();
        auto seq     = _func.iterator();

        if (auto symptr = as<symbol>(seq->next())) {
            auto sym = *symptr;

            if (sym == mSf_quote) {
                return val(sexp(seq));
            }

            if (sym == mSf_do) {
                val ret;

                while (auto v = seq->next()) {
                    ret = eval(*v);
                }
                return ret;
            }

            if (sym == mSf_comment) {
                return val();
            }

            if (sym == mSf_if) {
                assert(seq->remaining() >= 2);
                val ret;
                auto pred     = seq->next();
                auto is_true  = seq->next();
                auto is_false = seq->next();

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

        auto evaled = eval_to_exp(*this, *sexp_it.get());
        auto it     = evaled.iterator();

        return apply(*it, mEnvironment);
    }

    void Evaluator::replaceNode(ast::val const& _val) {
        assert(false);
    }

    ast::val const & Evaluator::currentNode() const {
        assert(false);
    }

    void Evaluator::enumerateBindings(
        std::function<void(ast::symbol const&, ast::val const&)> _func) const {
        mEnvironment.enumerate([_func](uint64_t id, ast::val const& _val) {
            ast::symbol sym { 0, id };
            _func(sym, _val);
        });
    }

    void Evaluator::enumerateBindings(
        std::function<void(std::string const&, ast::val const&)> _func) const {

        enumerateBindings(
            [this, &_func](ast::symbol const& sym, ast::val const& _val) {
                _func(symbolToName(sym), _val);
            });
    }

    val Evaluator::eval(val const& _v) {
        using namespace ast;

        val ret;

        if (_v.is<symbol>())
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
        iterator_base_t& _it, char const* intersperse, bool _add_types) const {
        return glisp::to_string(*this, _it, intersperse, _add_types);
    }

    std::string Evaluator::symbolToName(ast::symbol const& _sym) const {
        auto ret = mAllSymbols.getSymbolName({ _sym.mScope,_sym.mId });
        if(!ret) {
            fmt::print("Cannot find sym name {}:{}\n", _sym.mScope, _sym.mId);
            mAllSymbols.dump();
        }

        assert(ret);
        return *ret;
    }

    Evaluator::Evaluator()
        : mCallDepth(0)
          , mAllSymbols("glisp")
        , mReader(mAllSymbols) {

        mSf_if      = mAllSymbols.registerSymbol("if");
        mSf_and     = mAllSymbols.registerSymbol("and");
        mSf_or      = mAllSymbols.registerSymbol("or");
        mSf_quote   = mAllSymbols.registerSymbol("quote");
        mSf_do      = mAllSymbols.registerSymbol("do");
        mSf_comment = mAllSymbols.registerSymbol("comment");

        glisp::add_natives(*this);
    }

}
