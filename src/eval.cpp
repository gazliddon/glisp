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

    template <typename T>
    bool isSame(val const& _v) {
        if (_v.get().type() == typeid(T)) {
            return true;
        }

        return _v.get().type() == typeid(forward_ast<T>);
    }

    template <typename T>
    bool doTo(val const& _v, std::function<void(T const&)> _func) {
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
    const auto ret = val(list());

    val Evaluator::operator()(define const& _v) {
        val ret = boost::apply_visitor(*this, _v.mVal);
        mEnv.add(_v.mSym.mName, ret);
        return val(list());
    }

    val Evaluator::operator()(ast::boolean const&) {
        std::cout << "boolean" << std::endl;
        return ret;
    }
    val Evaluator::operator()(ast::symbol const& _v) {
        std::cout << "symbol" << std::endl;
        return ret;
    }
    val Evaluator::operator()(ast::keyword const& _keyword) {
        std::cout << "keyword" << std::endl;
        return ret;
    }
    val Evaluator::operator()(std::string const& _v) {
        std::cout << "string" << std::endl;
        return ret;
    }
    val Evaluator::operator()(ast::hint const& _hint) {
        std::cout << "hint" << std::endl;
        return ret;
    }
    val Evaluator::operator()(ast::nil const&) {
        std::cout << "nil" << std::endl;
        return ret;
    }
    val Evaluator::operator()(double _v) {
        std::cout << "double" << std::endl;
        return ret;
    }
    val Evaluator::operator()(char _v) {
        std::cout << "char" << std::endl;
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


    val Evaluator::eval(val const& _v) {

        namespace x3 = boost::spirit::x3;

        using namespace ast;

        val ret;

        if (isSame<forward_ast<define>>(_v)) {
            auto& val = boost::get<forward_ast<define>>(_v);
        }
        if (isSame<forward_ast<lambda>>(_v)) {
            auto& val = boost::get<forward_ast<lambda>>(_v);
        }
        else {
            std::cout << "Could not eval " << _v.get().type().name()
                      << std::endl;
        }

        return ret;
    }

    ////////////////////////////////////////////////////////////////////////////////

}
