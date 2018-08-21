#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <list>
#include <map>
#include <set>
#include <string>

/* #include <boost/config/warning_disable.hpp> */
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast {
    namespace x3 = boost::spirit::x3;
    using x3::forward_ast;

    enum eSpecialForm {
        Define,
        Quote,
        If,
        And,
        Or,
    };

    struct nil {};

    struct application;
    struct vector;
    struct map;
    struct meta;
    struct set;
    /* struct fn; */

    struct sp_define;
    struct sp_if;

    struct symbol {
        char mStart;
        std::string mName;
        std::string get() const {
            return mStart + mName;
        }
    };

    struct boolean {
        std::string mVal;
    };

    struct keyword {
        symbol mSym;
    };

    struct hint {
        symbol mSym;
    };

    struct special {
        eSpecialForm mForm;
        std::string mName;
    };

    // clang-format off

    struct val : x3::variant<
                 special
                 , boolean
                 , symbol
                 , std::string
                 , double
                 , char
                 , forward_ast<application>
                 , forward_ast<sp_define>
                 , forward_ast<sp_if>
                  > {

        using base_type::base_type;
        using base_type::operator=;
    };

    // clang-format on
    //
    struct sp_define {
        std::string mString;
        symbol mSym;
        val mVal;
    };

    struct sp_if {
        std::string mString;
        val mPred, mTrue, mFalse;
    };

    struct application {
        val mFunc;
        std::vector<val> mForms;
    };

    struct vector {
        std::vector<val> mForms;
    };

    struct map_entry {
        val mKey;
        val mValue;
    };

    struct map {
        std::list<map_entry> mHashMap;
    };

    struct meta {
        std::list<map_entry> mHashMap;
    };

    struct set {
        std::vector<val> mForms;
    };

    struct program {
        std::vector<val> mForms;
    };

    // print function for debugging
    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil";
        return out;
    }
} // namespace ast

#endif /* end of include guard: AST_H_4GSXUIIF */
