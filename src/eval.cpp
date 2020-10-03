#include "eval.h"

#include <iostream>

namespace ast {
    using boost::apply_visitor;
    using namespace std;

    namespace x3 = boost::spirit::x3;

    val Evaluator::eval(program const& _v) {
        val ret;

        for (auto const& form : _v.mForms) {
            ret = apply_visitor(*this, form);
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

    const auto ret = val(list());

    val Evaluator::operator()(define const& _v) {
        auto sym = _v.mSym.get();

        auto ret = _v.mVal;

        if (!_v.mVal.is_atom()) {
            ret = eval(_v.mVal);
        }

        mEnv = mEnv.set(sym, ret);

        return ret;
    }

    val Evaluator::operator()(ast::boolean const&) {
        assert(false);
        return ret;
    }

    val Evaluator::operator()(ast::symbol const& _v) {
        auto ret = mEnv.find(_v.get());

        if (ret == nullptr) {
            std::cout << "Could not find " << _v.get() << std::endl;
        }

        assert(ret != nullptr);
        return *ret;
    }

    val Evaluator::operator()(ast::keyword const& _keyword) {
        assert(false);
        return ret;
    }

    val Evaluator::operator()(std::string const& _v) {
        assert(false);
        return ret;
    }

    val Evaluator::operator()(ast::hint const& _hint) {
        assert(false);
        return ret;
    }

    val Evaluator::operator()(ast::nil const&) {
        assert(false);
        return ret;
    }
    val Evaluator::operator()(double _v) {
        return val(_v);
    }
    val Evaluator::operator()(char _v) {
        assert(false);
        return ret;
    }

    val Evaluator::operator()(ast::set const& _set) {
        std::cout << "set" << std::endl;
        return ret;
    }

    val Evaluator::operator()(ast::list const& _list) {
        std::cout << "list" << std::endl;
        return ret;
    }

    val Evaluator::operator()(ast::vector const& _vector) {
        std::cout << "vector" << std::endl;
        return ret;
    }

    val Evaluator::operator()(ast::map const& _map) {
        std::cout << "map" << std::endl;
        return ret;
    }

    val Evaluator::operator()(ast::meta const& _value) {
        std::cout << "meta" << std::endl;
        return ret;
    }

    val Evaluator::operator()(ast::map_entry const& _map_entry) {
        std::cout << "map_entry" << std::endl;
        return ret;
    }

    val Evaluator::operator()(ast::lambda const& _lambda) {
        std::cout << "lambda" << std::endl;
        return ret;
    }

    val const& Evaluator::eval(val const& _v) const {
        assert(false);
    }

    val Evaluator::operator()(ast::native_function const& _lambda) {
        assert(false);
    }

    val Evaluator::operator()(ast::function const& _func) {
        val ret;

        std::cout << "Evaluating function" << std::endl;

        auto nArgs = _func.mArgs.size();

        if (auto sym = _func.mFunc.get_val<symbol>()) {

            auto name = sym->get();

            if (name == "if") {
                assert(nArgs == 3);

                auto predicate = apply_visitor(*this, _func.mArgs[0]);

                if (predicate.to_bool()) {
                    return apply_visitor(*this, _func.mArgs[1]);
                } else {
                    return apply_visitor(*this, _func.mArgs[2]);
                }
            }

            if (name == "or") {
                assert(nArgs == 2);
                assert(false);
            }

            if (name == "and") {
                assert(nArgs == 2);
                assert(false);
            }

            if (name == "list") {
                assert(false);
            }

            if (name == "quote") {
                assert(false);
            }
        }

        auto func = apply_visitor(*this, _func.mFunc);

        std::vector<val> vals;
        vals.reserve(_func.mArgs.size());

        for (auto const& v : _func.mArgs) {
            vals.push_back(apply_visitor(*this, v));
        }

        if (auto nf = func.get_val<native_function>()) {
            return nf->mFunc(mEnv, vals);
        }

        if (auto nf = func.get_val<lambda>()) {
            auto env = mEnv;
            assert(nf->mArgs.size() <= _func.mArgs.size());
            auto i = 0;

            for (auto const& a : nf->mArgs) {
                mEnv = mEnv.set(a.get(), vals[i++]);
            }

            auto retVal = eval(nf->mBody);

            mEnv = env;
            return retVal;
        }


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
