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

    struct symbol {
        char mFirst;
        std::string mRest;

        std::string get() const {
            return mFirst + mRest;
        }
    };

    struct form : x3::variant<nil,
                              symbol,
                              std::string,
                              unsigned int,
                              x3::forward_ast<list>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    typedef std::list<form> form_list;

    struct list {
        form_list mForms;
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
