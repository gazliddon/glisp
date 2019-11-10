#include "eval.h"

#include "printer2.h"

namespace ast2 {
    using boost::apply_visitor;
    using namespace std;

    // helpers

    static bool valToBool(value_t const& _val) {
        bool ret = false;

        if (auto truth = boost::get<truthy_t>(&_val)) {
            ret = truth->mValue;
        }
        return ret;
    }

    static bool doesThisEval(value_t const& _val) {
        return apply_visitor(does_eval{}, _val);
    }

    static bool isListEvaled(list_t const& _lst) {
        bool allEvalled = true;

        for (auto const& i : _lst.mData) {
            allEvalled &= doesThisEval(i);
        }

        return allEvalled;
    }

    template <typename T>
    bool isSame(value_t const& _v) {
        if (_v.get().type() == typeid(T)) {
            return true;
        }

        return _v.get().type() == typeid(forward_ast<T>);
    }

    template <typename T>
    bool doTo(value_t const& _v, std::function<void(T const&)> _func) {
        auto okay = isSame<T>(_v);
        if (okay) {
            if (_v.get().type() == typeid(T)) {
                _func(boost::get<T>(_v));
            } else {
                _func(boost::get<forward_ast<T>>(_v));
            }
        }
        return okay;
    }

    ////////////////////////////////////////////////////////////////////////////////

    value_t Evaluator::eval(value_t const& _v) {
        if (doesThisEval(_v)) {
            return boost::apply_visitor(*this, _v);
        } else {
            auto ret = _v;
            return ret;
        }
    }

    value_t Evaluator::apply(proc_t const& _p, list_t const& _args) {
        cout << "About to apply" << endl;
        auto evalled = isListEvaled(_args);
        assert(evalled);
        return _p.mFunc(mEnv, _args);
    }

    forward_ast<proc_t> const* asProc(value_t const& _val) {
        return boost::get<forward_ast<proc_t>>(&_val);
    }

    value_t Evaluator::operator()(list_t const& _l) {
        cout << "List eval" << endl;

        /* list_t evaled; */

        /* for (auto const& i : _l.mData) { */
        /*     auto v = eval(i); */
        /*     evaled.mData.push_back(v); */
        /* } */

        /* auto proc = evaled.car(); */
        /* auto args = evaled.cdr(); */

        /* if (auto p = asProc(proc)) { */
        /*     return apply(*p, args); */
        /* } */

        assert(false);
    }

    value_t Evaluator::operator()(if_t const& _val) {
        auto pred = eval(_val.mPred);

        if (valToBool(pred)) {
            return eval(_val.mA);
        } else {
            return eval(_val.mB);
        }
    }

    value_t Evaluator::operator()(and_t const&) {
        assert(false);
    }
    value_t Evaluator::operator()(or_t const&) {
        assert(false);
    }

    value_t Evaluator::operator()(define_t const& _val) {
        mEnv.add(_val.mSym.mName, eval(_val.mVal));
        return _val.mSym;
    }

    value_t Evaluator::operator()(quote_t const&) {
        assert(false);
    }

    // No eval for these
    value_t Evaluator::operator()(proc_t const& _p) {
        return _p;
    }

    value_t Evaluator::operator()(symbol_t const& _sym) {
        assert(mEnv.isDefined(_sym.mName));
        return mEnv.get(_sym.mName);
    }

    value_t Evaluator::operator()(truthy_t const&) {
        assert(false);
    }

    value_t Evaluator::operator()(error_t const&) {
        assert(false);
    }

    value_t Evaluator::operator()(nil_t const&) {
        assert(false);
    }

    value_t Evaluator::operator()(std::string const&) {
        assert(false);
    }

    value_t Evaluator::operator()(double const&) {
        assert(false);
    }

    value_t Evaluator::operator()(char const&) {
        assert(false);
    }
}

namespace ast2 {

    bool does_eval::operator()(symbol_t const&) const {
        return true;
    }

    bool does_eval::operator()(truthy_t const&) const {
        return false;
    }

    bool does_eval::operator()(list_t const&) const {
        return true;
    }

    bool does_eval::operator()(proc_t const&) const {
        return false;
    }

    bool does_eval::operator()(if_t const&) const {
        return true;
    }

    bool does_eval::operator()(and_t const&) const {
        assert(false);
    }
    bool does_eval::operator()(or_t const&) const {
        assert(false);
    }

    bool does_eval::operator()(define_t const&) const {
        return true;
    }

    bool does_eval::operator()(quote_t const&) const {
        assert(false);
    }

    bool does_eval::operator()(error_t const&) const {
        assert(false);
    }

    bool does_eval::operator()(nil_t const&) const {
        return false;
    }

    bool does_eval::operator()(std::string const&) const {
        return false;
    }

    bool does_eval::operator()(double const&) const {
        return false;
    }

    bool does_eval::operator()(char const&) const {
        return false;
    }
}


namespace ast2 {

    value_t println(env_t& _env, list_t const& args) {
        return {};
    }

    void testEval() {
        using namespace std;
        Printer p(cout);
    }
}
