#include "run.h"

// Datum is a lisp construct that will not evaluate
// a normal form is a self evaluating datum
// a compound form will evaluate to a to a normal form
// Special forms (or syntax) are forms that evaluate with special rules
//
// So!
// evaluate a list from left to right to get a compound form
// apply
//  eval head - is it a procedure? yes
//  evaluate args
//  call procedure with args

////////////////////////////////////////////////////////////////////////////////
#include <iostream>

using std::cout;
using std::endl;

////////////////////////////////////////////////////////////////////////////////
//

value_t car(list_t _list);
list_t cdr(list_t _list);

////////////////////////////////////////////////////////////////////////////////

value_t apply(env_t env, procedure_t, list_t _list) {
    assert(false);
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

truthy_t asTruthy(value_t const& v) {
    if (auto pTruth = boost::get<truthy_t>(&v)) {
        return *pTruth;
    } else {
        return truthy_t::False;
    }
}

bool asBool(value_t const& v) {
    return asTruthy(v) == truthy_t::True;
}

struct Evaluator : public boost::static_visitor<value_t> {

    value_t syntax_if(value_t const& pred, value_t const& a, value_t const& b) {
        cout << "syntax if" << endl;
        auto evaled = eval(pred);

        if (asBool(evaled)) {
            return eval(a);
        } else {
            return eval(b);
        }
    }

    value_t syntax_define(gsymbol_t const& _sym, value_t const& _value) {
        assert(false);
        return nil_t{};
    }
    value_t syntax_or(list_t const& args) {
        assert(false);
    }

    value_t syntax_and(list_t const& args) {
        value_t last = truthy_t::True;

        for (auto const& i : args) {
            last = eval(i);
            if (!asBool(last)) {
                return truthy_t::False;
            }
        }
        return last;
    }

    value_t syntax(syntax_t syn, list_t args) {
        using boost::get;

        switch (syn) {
            case If: {
                assert(args.size() == 2);
                return syntax_if(args[0], args[1], args[2]);
            }
            case And:
                return syntax_and(args);

            case Or:
                return syntax_or(args);

            case Define: {
                assert(args.size() == 2);
                if (auto sym = boost::get<gsymbol_t>(&args[1])) {
                    return syntax_define(*sym, args[1]);
                } else {
                    assert(false);
                }
            }

            case Quote:
                return cdr(args);
                break;
        }
    }

    value_t eval(value_t const& v) {
        return boost::apply_visitor(*this, v);
    }

    value_t operator()(list_t const& val) {

        cout << "Evalling list" << endl;
        // Empty list evals to nil
        if (val.empty()) {
            cout << "empty" << endl;
            return nil_t{};
        }

        // Is this syntax?
        if (auto pSyn = boost::get<syntax_t>(&val[0])) {
            cout << "syntax" << endl;
            // own rulez
            return syntax(*pSyn, cdr(val));
        }

        // Nope, no syntax list!
        list_t evaled;

        cout << "evalling all list and args" << endl;
        for (auto const& item : val) {
            evaled.push_back(eval(item));
        }

        // Apply if we can
        if (auto pProc = boost::get<procedure_t>(&evaled[0])) {
            return apply(mEnv, *pProc, cdr(evaled));
        }

        assert(false);
    }

    value_t operator()(gsymbol_t const& _sym) {
        return mEnv.lookup(_sym, _sym);
    }

    template <typename T>
    value_t operator()(T const& val) {
        return val;
    }

    env_t mEnv;
};

////////////////////////////////////////////////////////////////////////////////
value_t car(list_t _list) {
    return _list.front();
}

list_t cdr(list_t _list) {
    if (_list.size() > 1) {
        assert(false);
    } else {
        return _list;
    }
}

////////////////////////////////////////////////////////////////////////////////
void test(void) {

    

    cout << "1" << endl;

    auto l = std::vector<value_t>{
        syntax_t::If,   // j
        truthy_t::True, //
        1.0,            //
        0.0,            //
    };

    cout << "2" << endl;

    /* value_t val(l); */

    /* list_t_template<value_t> l(immer::vector<value_t>{ */
    /*         syntax_t::If, */
    /*         truthy_t::True, */
    /*         "YES", */
    /*         "NO" */
    /*         }); */

    Evaluator e;

    cout << "3" << endl;

    auto lst = list_t(l);

    cout << "4" << endl;

    auto res = e.eval(lst);



    if (auto* r = boost::get<double>(&res)) {
        cout << (*r) << endl;
    } else {
        cout << "dunno" << endl;
    }
}
