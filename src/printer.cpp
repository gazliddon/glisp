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

    void print(glisp::reader_reslult_t const& _p, std::ostream& _out) {
        print(_p.mAst, _out);
    }

    /* void printer::operator()(ast::val const& _v) const { */
    /*     assert(false); */

    /* } */

    printer::printer(std::ostream& _out, bool print_types)
        : printer_base(_out), mPrintTypes(print_types) {
    }

    void printer::operator()(ast::program const& _program) const {
        renderCollection(_program.mForms,"","","\n");
    }

    void printer::operator()(ast::sexp const& _func) const {
        mOut << "(";
        renderCollection(_func.mForms);
        mOut << ")";
    }

    void printer::operator()(ast::lambda const& _lambda) const {
        mOut << "(lambda ";
        renderVector(_lambda.mArgs);
        mOut << " ";
        render(_lambda.mBody);
        mOut << ")";
    }

    void printer::operator()(ast::keyword const& _keyword) const {
        mOut << ":" << _keyword.mSym.mName << "";
    }

    void printer::operator()(ast::hint const& _keyword) const {
        mOut << "^" << _keyword.mSym.mName << "";
    }

    void printer::operator()(bool const& _val) const {
        if (_val) {
            mOut << "true";
        } else {

            mOut << "false";
        }
        mOut << "";
    }

    void printer::operator()(char _v) const {
        mOut << _v << "";
    }

    void printer::operator()(ast::nil const&) const {
        mOut << "nil";
    }

    void printer::operator()(double _v) const {
        mOut << _v << "";
    }

    void printer::operator()(std::string const& _v) const {
        mOut << "\"" << _v << "\""
             << "";
    }

    void printer::operator()(ast::symbol const& _v) const {
        mOut << _v.mName << "";
    }

    void printer::operator()(ast::vector const& _vector) const {
        renderCollection(_vector.mForms, "[", "]");
        mOut << "";
    }

    void printer::operator()(native_function const& _func) const {
        mOut << ":native function with " << _func.mNumOfArgs << " args";
    }

    void printer::operator()(ast::map const& _map) const {
        renderCollection(_map.mHashMap, "{", "}", "");
    }

    void printer::operator()(ast::meta const& _val) const {
        renderCollection(_val.mHashMap, "^{", "}", "");
    }

    void printer::operator()(ast::set const& _set) const {
        renderCollection(_set.mForms, "#{", "}", "");
    }

    void printer::operator()(ast::map_entry const& _map_entry) const {
        render(_map_entry.mKey);
        mOut << " ";
        render(_map_entry.mValue);
    }
}
