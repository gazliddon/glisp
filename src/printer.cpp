#include "printer.h"
#include <stdio.h>

#include <iostream>

namespace ast {
    using boost::apply_visitor;


    /* void printer::operator()(ast::val const& _v) const { */
    /*     assert(false); */

    /* } */

    printer::printer(std::ostream& _out)
        : printer_base(_out) {
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
        mOut << _v << ":string";
    }

    void printer::operator()(ast::symbol const& _v) const {
        mOut << _v.get() << ":symbol";
    }

    void printer::operator()(ast::special const& _v) const {
        mOut << _v.mName << ":special-form";
    }

    void printer::operator()(ast::application const& _list) const {
        mOut << "(";
        boost::apply_visitor(*this, _list.mFunc);

        mOut << ":func";

        if (!_list.mForms.empty()) {
            mOut << " ";
            renderCollection(_list.mForms);
        }

        mOut << "):application";
    }

    void printer::operator()(ast::vector const& _vector) const {
        renderCollection(_vector.mForms, "[", "]", "vector");
    }

    void printer::operator()(ast::map const& _map) const {
        /* renderCollection(_map.mHashMap, "{", "}", "map"); */
    }

    void printer::operator()(ast::meta const& _val) const {
        /* renderCollection(_val.mHashMap, "^{", "}", "meta"); */
    }

    void printer::operator()(ast::set const& _set) const {
        renderCollection(_set.mForms, "#{", "}", "set");
    }

    void printer::operator()(ast::sp_define const& _define) const {
        mOut<< "(define ";
        (*this)(_define.mSym);
        mOut << " ";
        boost::apply_visitor(*this, _define.mVal);
        mOut << "):special";
    }

    void printer::operator()(ast::map_entry const& _map_entry) const {
        assert(false);
        /* apply_visitor(*this, _map_entry.mKey); */
        /* mOut << " "; */
        /* apply_visitor(*this, _map_entry.mValue); */
    }
}
