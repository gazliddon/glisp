#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <string>
#include <set>
#include <map>
#include <list>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast {
    namespace x3 = boost::spirit::x3;
    using x3::forward_ast;

    struct nil {};

    struct list;
    struct vector;
    struct map;
    struct set;
    struct special_form;
    struct fn;

    struct symbol {
        char mFirst;
        std::string mRest;

        std::string get() const {
            return mFirst + mRest;
        }
    };

    struct keyword {
        symbol mSym;
    };

    struct atom : x3::variant<symbol,
                              keyword,
                              std::string,
                              x3::forward_ast<fn>,
                              nil,
                              unsigned int,
                              double,
                              bool,
                              char> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct form : x3::variant<atom,
                              x3::forward_ast<special_form>,
                              x3::forward_ast<set>,
                              x3::forward_ast<list>,
                              x3::forward_ast<vector>,
                              x3::forward_ast<map>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    typedef std::list<form> form_list;

    struct special_form {
        std::string mName;
        form_list mForms;
    };

    struct map_entry {
        form mKey;
        form mValue;
    };

    typedef std::list<map_entry> map_list;

    struct list {
        form_list mForms;
    };

    struct vector {
        form_list mForms;
    };

    struct map {
        map_list mHashMap;
    };

    struct set {
        form_list mForms;
    };

    struct fn {
        std::function< void(form_list const & _args) > mFn;
        size_t mNumberOfArgs;
    };

    struct program {
        form_list mForms;
    };

    // print function for debugging
    inline std::ostream &operator<<( std::ostream &out, nil ) {
        out << "nil";
        return out;
    }
}

#endif /* end of include guard: AST_H_4GSXUIIF */
