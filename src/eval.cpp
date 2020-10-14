#include "eval.h"
#include "printer.h"

#include <iostream>

namespace ast {
    using boost::apply_visitor;
    using namespace std;

    namespace x3 = boost::spirit::x3;

    val Evaluator::operator()(ast::macro const& _macro) {
        auto& sym = _macro.mSym.mName;
        mEnv      = mEnv.set(sym, ast::val(_macro));
        return ast::val(_macro.mSym);
    }

    val Evaluator::eval(program const& _v) {
        val ret;

        for (auto const& form : _v.mForms) {
            ret = eval(form);
        }

        return ret;
    }

    // helpers

    /* static bool valToBool(val const& _val) { */
    /*     bool ret = false; */

    /*     if (auto truth = boost::get<boolean>(&_val)) { */
    /*         ret = false; */
    /*         assert(!"fucked"); */
    /*     } */
    /*     return ret; */
    /* } */

    /* static bool doesThisEval(val const& _val) { */
    /*     /1* return apply_visitor(does_eval{}, _val); *1/ */
    /*     return false; */
    /* } */

    /* static bool isListEvaled(list const& _lst) { */
    /*     bool allEvalled = true; */

    /*     for (auto const& i : _lst.mForms) { */
    /*         allEvalled &= doesThisEval(i); */
    /*     } */

    /*     return allEvalled; */
    /* } */

    val Evaluator::operator()(define const& _v) {
        auto& sym = _v.mSym.mName;

        auto ret = _v.mVal;

        if (!_v.mVal.is_atom()) {
            ret = eval(_v.mVal);
        }

        mEnv = mEnv.set(sym, ret);

        return val(_v.mSym);
    }

    val Evaluator::operator()(bool const& _val) {
        assert(false);
    }

    val Evaluator::operator()(ast::symbol const& _v) {
        auto ret = mEnv.find(_v.mName);

        if (ret == nullptr) {
            std::cout << "Could not find " << _v.mName << std::endl;
        }

        assert(ret != nullptr);
        return *ret;
    }

    val Evaluator::operator()(ast::keyword const& _keyword) {
        assert(false);
        return val(nil());
    }

    val Evaluator::operator()(std::string const& _v) {
        assert(false);
        return val(nil());
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
        return val(_v);
    }
    val Evaluator::operator()(char _v) {
        assert(false);
        return val(nil());
    }

    val Evaluator::operator()(ast::set const& _set) {
        std::cout << "set" << std::endl;
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
        std::cout << "map" << std::endl;
        assert(false);
        return val(nil());
    }

    val Evaluator::operator()(ast::meta const& _value) {
        std::cout << "meta" << std::endl;
        assert(false);
        return val(nil());
    }

    val Evaluator::operator()(ast::map_entry const& _map_entry) {
        std::cout << "map_entry" << std::endl;
        assert(false);
        return val(nil());
    }

    val Evaluator::operator()(ast::lambda const& _lambda) {
        std::cout << "lambda" << std::endl;
        assert(false);
        return val(nil());
    }

    val const& Evaluator::eval(val const& _v) const {
        assert(false);
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
        val ret;

        auto const& exps = _func.mForms;
        auto first       = get_first(exps);

        if (auto sym = first.get_val<symbol>()) {

            auto args  = get_rest(_func.mForms);
            auto nArgs = args.size();
            auto& name = sym->mName;

            if (name == "type") {
                if (nArgs != 1) {
                    cout << "nargs " << nArgs << endl;
                    /* assert(nArgs == 1); */
                }
                ast::type_getter_t t;
                auto x = t(eval(args[0]));
                return ast::val(x);
            }

            if (name == "do") {

                auto ret = ast::val(ast::nil());

                for (auto const& v : args) {
                    ret = eval(v);
                }
                return ret;
            }

            if (name == "list") {
                for (auto& v : args) {
                    v = eval(v);
                }
                sexp ret = { .mForms = args };
                return val(ret);
            }

            if (name == "vec") {
                for (auto& v : args) {
                    v = eval(v);
                }
                vector ret = { .mForms = args };
                return val(ret);
            }

            if (name == "if") {
                assert(nArgs == 3);

                auto predicate = eval(args[0]);

                if (predicate.to_bool()) {
                    return eval(args[1]);
                } else {
                    return eval(args[2]);
                }
            }

            if (name == "or") {
                for (auto const& v : args) {
                    if (eval(v).to_bool()) {
                        return val(true);
                    }
                }
                return val(false);
            }

            if (name == "and") {
                for (auto const& v : args) {
                    if (!eval(v).to_bool()) {
                        return val(false);
                    }
                }
                return val(true);
            }

            if (name == "quote") {
                sexp ret = { .mForms = args };
                return val(ret);
            }

            first = eval(first);

            for (auto& v : args) {
                v = eval(v);
            }

            if (auto nf = first.get_val<native_function>()) {
                return nf->mFunc(mEnv, args);
            }

            if (auto nf = first.get_val<lambda>()) {
                auto env = mEnv;
                assert(args.size() == nf->mArgs.size());
                auto i = 0;

                for (auto const& a : nf->mArgs) {
                    mEnv = mEnv.set(a.mName, args[i++]);
                }

                auto retVal = eval(nf->mBody);

                mEnv = env;
                return retVal;
            }

            if (auto p = first.get_val<macro>()) {
                cout << "unexpanded MACRO " << p->mSym.mName << endl;
                return ast::val(*p);
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
