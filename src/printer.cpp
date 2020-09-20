#include "printer.h"
#include <stdio.h>

#include <iostream>

namespace ast {
    using boost::apply_visitor;
    void print(ast::program const& _p, std::ostream& _out) {
        if (_p.mForms.size() != 0) {
            ast::printer printIt(_out);
            for (auto const& i : _p.mForms) {
                boost::apply_visitor(printIt, i);
                _out << std::endl;
            }
            _out << std::endl;
        }
    }


    /* void printer::operator()(ast::val const& _v) const { */
    /*     assert(false); */

    /* } */

    printer::printer(std::ostream& _out)
        : printer_base(_out) {
    }
    
    void printer::operator()(ast::lambda const& _lambda) const {
        mOut << "(" << "lambda";
        renderVector(_lambda.mArgs, "args");
        mOut << " ";
        (*this)(_lambda.mBody);
        mOut << "):lambda";
    }

    void printer::operator()(ast::keyword const& _keyword) const {
        mOut << ":" << _keyword.mSym.get() << ":keyword";
    }

    void printer::operator()(ast::hint const& _keyword) const {
        mOut << "^" << _keyword.mSym.get() << ":typehint";
    }

    void printer::operator()(ast::boolean const& _val) const {
        mOut << _val.mVal << ":bool";
    }

    void printer::operator()(char _v) const {
        mOut << _v << ":char";
    }

    void printer::operator()(ast::nil const&) const {
        BOOST_ASSERT(0);
    }

    void printer::operator()(double _v) const {
        mOut << _v << ":double";
    }

    void printer::operator()(std::string const& _v) const {
        mOut << "\"" << _v << "\"" << ":string";
    }

    void printer::operator()(ast::symbol const& _v) const {
        mOut << _v.get() << ":symbol";
    }


    void printer::operator()(ast::list const& _list) const {
        mOut << "(";

        if (!_list.mForms.empty()) {
            mOut << " ";
            renderCollection(_list.mForms);
        }

        mOut << "):application";
    }

    void printer::operator()(ast::vector const& _vector) const {
        renderCollection(_vector.mForms, "[", "]");
        mOut << ":vector";
    }

    void printer::operator()(native_function const & _func) const {
        mOut << ":native function with " << _func.mNumOfArgs << " args";
    }

    void printer::operator()(ast::map const& _map) const {
        renderCollection(_map.mHashMap, "{", "}", "map");
    }

    void printer::operator()(ast::meta const& _val) const {
        renderCollection(_val.mHashMap, "^{", "}", "meta");
    }

    void printer::operator()(ast::set const& _set) const {
        renderCollection(_set.mForms, "#{", "}", "set");
    }

    void printer::operator()(ast::map_entry const& _map_entry) const {
        (*this)(_map_entry.mKey);
        mOut << " ";
        (*this)(_map_entry.mValue);
    }
}
