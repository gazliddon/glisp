#ifndef PRINTER_H_9ABCZ08P
#define PRINTER_H_9ABCZ08P

#include "ast.h"
#include <assert.h>
#include <iostream>
#include <string>

namespace ast {

struct printer_base {
    typedef void result_type;
    std::ostream& mOut;
    int mIndent;
    printer_base(std::ostream& _out)
        : mOut(_out)
        , mIndent(-1) {
    }
};

struct type_getter_t : boost::static_visitor<std::string> {
    std::string operator()(bool const&) const {
        return "bool";
    }

    std::string operator()(ast::symbol const& _v) const {
        return "symbol";
    }

    std::string operator()(ast::keyword const& _keyword) const {
        return "keyword";
    }

    std::string operator()(std::string const& _v) const {
        return "string";
    }

    std::string operator()(ast::hint const& _hint) const {
        return "hint";
    }

    std::string operator()(ast::nil const&) const {
        return "nil";
    }

    std::string operator()(double _v) const {
        return "double";
    }

    std::string operator()(char _v) const  {
        return "char";
    }

    std::string operator()(ast::set const& _set) const {
        return "set";
    }


    std::string operator()(ast::vector const& _vector) const {
        return "vector";
    }

    std::string operator()(ast::map const& _map) const {
        return "map";
    }

    std::string operator()(ast::meta const& _value) const {
        return "meta";
    }

    std::string operator()(ast::map_entry const& _map_entry) const {
        return "map_entry";
    }

    std::string operator()(ast::lambda const& _lambda) const {
        return "_fn";
    }

    std::string operator()(ast::native_function const& _lambda) const {
        return "fn";
    }

    std::string operator()(ast::sexp const& _func) const {
        return "sexp";
    }

    std::string operator()(ast::program const& _program) const {
        return "block";
    }

    std::string operator()(ast::macro const& _mac) const {
        return "macro";
    }

    std::string operator()(ast::define const& _def) const {
        return "define";
    }

    std::string operator()(ast::val const& _val) const {
        return boost::apply_visitor(*this, _val);
    }

    std::string operator()(ast::arg const&) const {
        return "arg";
    }
    std::string operator()(ast::let const&) const {
        return "let";
    }
};

struct printer : boost::static_visitor<void>, printer_base {
    bool mPrintTypes;
    printer(std::ostream& _out = std::cout, bool print_types = false);

    template<typename T>
        void render(T const & _val) const {
        (*this)(_val);
        }

    void operator()(bool const&) const;
    void operator()(ast::symbol const& _v) const;
    void operator()(ast::keyword const& _keyword) const;
    void operator()(std::string const& _v) const;
    void operator()(ast::hint const& _hint) const;
    void operator()(ast::nil const&) const;
    void operator()(double _v) const;
    void operator()(char _v) const;
    void operator()(ast::set const& _set) const;
    void operator()(ast::vector const& _vector) const;
    void operator()(ast::map const& _map) const;
    void operator()(ast::meta const& _value) const;
    void operator()(ast::map_entry const& _map_entry) const;
    void operator()(ast::lambda const& _lambda) const;
    void operator()(ast::native_function const& _lambda) const;
    void operator()(ast::sexp const& _func) const;
    void operator()(ast::program const& _program) const;

    void operator()(ast::arg const& _val) const {
        mOut << _val.mSymbol.mName << " ";
        render(_val.mVal);
    }

    void operator()(ast::let const& _val) const {
        mOut << "(let ";
        renderVector(_val.mArgs);
        mOut << _val.mBody << ")";
    }

    void operator()(ast::macro const& _mac) const {
        mOut << "(defmacro " << _mac.mSym  << " ";
        renderVector(_mac.mArgs.mForms);
        mOut << " ";
        render(_mac.mVal);
        mOut << ")";
    }

    void operator()(ast::define const& _def) const {
        mOut << "(define " << _def.mSym.mName << " ";
        render(_def.mVal);
        mOut << ")";
    }

    void operator()(ast::val const& _val) const {
        boost::apply_visitor(*this, _val);
    }

    template <typename T>
    void renderCollection(T const& _col,
        char const* _pre         = "",
        char const* _post        = "",
        char const* _intersperse = " ") const {
        auto b = _col.begin();
        auto e = _col.end();
        mOut << _pre;
        while (b != e) {
            (*this)(*b++);
            if (b != e) {
                mOut << _intersperse;
            }
        }
        mOut << _post;
    }

    template <typename T>
    void renderList(T const& _col, std::string const& _type="") const {
        renderCollection(_col, "(", ")", _type);
    }

    template <typename T>
    void renderVector(T const& _col, std::string const& _type="") const {
        renderCollection(_col, "[", "]", _type);
    }

    template <typename T>
    void renderCollection(T const& _col,
        char const* _pre,
        char const* _post,
        std::string const& _type) const {
        renderCollection(_col, _pre, _post);
        if (!_type.empty())
            mOut << ":" << _type;
    }
};

template <typename T>
void print(T const& _ast, std::ostream& _out = std::cout) {
    ast::printer printIt(_out);
    printIt.render(_ast);
    _out << std::endl;
}

void print(ast::program const& _p, std::ostream& _out = std::cout);

} // namespace ast

#endif /* end of include guard: PRINTER_H_9ABCZ08P */
