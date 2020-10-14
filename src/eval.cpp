#include "eval.h"
#include "printer.h"
#include "except.h"

#include <iostream>
#include <spdlog/spdlog.h>

namespace ast {
    using boost::apply_visitor;
    using namespace std;

    namespace x3 = boost::spirit::x3;

    val Evaluator::operator()(ast::macro const& _macro) {
        auto macro_val = ast::val(_macro);
        set(_macro.mSym.mName, macro_val);
        return macro_val;
    }
    val Evaluator::operator()(ast::let const& _let) {

        auto oldEnv = mEnv;

        auto ret = val();

        for (auto const& arg : _let.mArgs) {
            set(arg.mSymbol.mName, eval(arg.mVal));
        }

        ret = eval(_let.mBody);

        mEnv = oldEnv;

        return ret;
    }

    val Evaluator::operator()(define const& _v) {
        auto& sym = _v.mSym.mName;

        auto ret = _v.mVal;

        if (!_v.mVal.is_atom()) {
            ret = eval(_v.mVal);
        }

        set(sym, ret);

        return val(_v.mSym);
    }

    val Evaluator::operator()(bool const& _val) {
        assert(false);
    }

    val Evaluator::operator()(ast::symbol const& _v) {
        auto ret = mEnv.find(_v.mName);

        if (ret == nullptr) {
            auto const & ctx = mReaderContext.top();

            auto x = fmt::format("Can't find symbol {}", _v.mName);

            throw( glisp::cEvalError(x.c_str()) );
        }

        return *ret;
    }

    val Evaluator::operator()(ast::keyword const& _keyword) {
        assert(false);
        return val();
    }

    val Evaluator::operator()(std::string const& _v) {
        assert(false);
        return val();
    }

    val Evaluator::operator()(ast::hint const& _hint) {
        assert(false);
        return val(nil());
    }

    val Evaluator::operator()(ast::nil const&) {
        assert(false);
        return val(nil());
    }
    val Evaluator::operator()(double _v) {
        assert(false);
        return val(nil());
    }
    val Evaluator::operator()(char _v) {
        assert(false);
        return val(nil());
    }

    val Evaluator::operator()(ast::set const& _set) {
        assert(false);
        return val(nil());
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

    val Evaluator::operator()(ast::meta const& _value) {
        std::cout << "meta" << std::endl;
        assert(false);
        return val(nil());
    }

    val Evaluator::operator()(ast::lambda const& _lambda) {
        std::cout << "lambda" << std::endl;
        assert(false);
        return val(nil());
    }

    val const& Evaluator::eval(val const& _v) const {
        if (_v.is_atom()) {
            return _v;
        } else {
            return eval(_v);
        }
    }

    val Evaluator::operator()(ast::native_function const& _lambda) {
        assert(false);
        return val(nil());
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
        auto ret = ast::val(ast::nil());

        for (auto const& f : _program.mForms) {
            ret = eval(f);
        }

        return ret;
    }

    std::vector<ast::val> get_rest(std::vector<ast::val> const& _args) {
        return get_rest(_args.begin(), _args.end());
    }

    ast::val get_first(std::vector<ast::val> const& _args) {
        return get_first(_args.begin(), _args.end());
    }

    val Evaluator::operator()(ast::sexp const& _func) {

        auto verbose = false;

        if (auto p = mEnv.find("*verbose*")) {
            if (auto b = p->get_val<bool>()) {
                verbose = *b;
            }
        }

        val ret;

        auto const& exps = _func.mForms;

        auto const firsti = exps.begin();
        auto const argsi  = firsti + 1;
        auto const argse  = exps.end();
        auto const nArgs  = exps.size() - 1;

        auto eval_args = [this, argsi, argse]() {
            std::vector<ast::val> ret(argsi, argse);

            for (auto& v : ret) {
                v = eval(v);
            }
            return ret;
        };

        auto reduce_args = [this, argsi, argse]() {
            auto ret = ast::val(ast::nil());
            for (auto i = argsi; i != argse; i++) {
                ret = eval(*i);
            }
            return ret;
        };

        auto map_args
            = [argsi, argse](std::function<bool(ast::val const&)> _func) {
                  for (auto i = argsi; i != argse; i++) {
                      if (_func(*i)) {
                          break;
                      }
                  }
              };

        if (auto nf = firsti->get_val<keyword>()) {

            assert(nArgs == 1);
            auto evaled_arg = eval(*argsi);

            if (auto mp = evaled_arg.get_val<map>()) {
                return mp->get(val(*nf));
            } else {
                return val();
            }
        }

        if (auto sym = firsti->get_val<symbol>()) {
            if (verbose) {
                cout << "first sexp arg is sybol " << sym->mName << endl;
            }

            auto& name = sym->mName;

            if (name == "cond") {
                assert((nArgs % 1) == 0);

                for (auto i = argsi; i != argse; i += 2) {
                    auto v = eval(*i);
                    if (v.to_bool()) {
                        return eval(*(i + 1));
                    }
                }

                return val();
            }

            if (name == "type") {
                if (nArgs != 1) {
                    cout << "nargs " << nArgs << endl;
                    /* assert(nArgs == 1); */
                }
                ast::type_getter_t t;
                auto x = t(eval(*argsi));
                return ast::val(x);
            }

            if (name == "do") {
                return reduce_args();
            }

            if (name == "list") {
                sexp ret(eval_args());
                return val(ret);
            }

            if (name == "vec") {
                vector ret(eval_args());
                return val(ret);
            }

            if (name == "apply") {
                assert(nArgs == 2);
                assert(!"TBD");
                return val(nil());
            }

            if (name == "if") {
                assert(nArgs == 3);

                auto predicate = eval(*argsi);

                if (predicate.to_bool()) {
                    return eval(*(argsi + 1));
                } else {
                    return eval(*(argsi + 2));
                }
            }

            if (name == "recur") {
                return val();
            }

            if (name == "or") {
                auto ret = false;

                map_args([this, &ret](auto const& v) {
                    ret = eval(v).to_bool();
                    return ret;
                });

                return val(ret);
            }

            if (name == "and") {
                auto ret = true;

                map_args([this, &ret](auto const& v) {
                    ret = eval(v).to_bool();
                    return !ret;
                });

                return val(ret);
            }

            if (name == "quote") {
                assert(nArgs == 1);
                return *argsi;
            }

            auto first = eval(*firsti);
            auto args  = eval_args();

            if (auto nf = first.get_val<native_function>()) {
                return nf->call(*this, args);
            }

            if (auto fn = first.get_val<lambda>()) {

                auto env = mEnv;

                assert(args.size() == fn->mArgs.size());

                auto i = 0;

                for (auto const& a : fn->mArgs) {
                    set(a.mName, args[i++]);
                }

                auto retVal = eval(fn->mBody);

                mEnv = env;

                return retVal;
            }
        }


        assert(!"can't eval");
        return ret;
    }

    val Evaluator::eval(val const& _v) {
        using namespace ast;
        val ret;

        if (_v.is_atom() || _v.is<lambda>())
            return _v;
        else
            return boost::apply_visitor(*this, _v);
    }

    ////////////////////////////////////////////////////////////////////////////////

}
