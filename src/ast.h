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

    struct nil {};

    struct application;
    struct vector;
    struct map;
    struct meta;
    struct set;
    /* struct fn; */

    struct sp_define;
    struct sp_if;
    struct sp_or;
    struct sp_and;
    struct sp_lambda;
    struct sp_quote;
    struct sp_list;

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


    // clang-format off

    struct val : x3::variant<
                 boolean
                 , symbol
                 , std::string
                 , double
                 , char
                 , forward_ast<application>
                 , forward_ast<sp_define>
                 , forward_ast<sp_if>
                 , forward_ast<sp_or>
                 , forward_ast<sp_and>
                 , forward_ast<sp_lambda>
                 , forward_ast<sp_quote>
                 , forward_ast<sp_list>
                  > {

        using base_type::base_type;
        using base_type::operator=;
    };

    // clang-format on
    //
    
    struct sp_list {
        std::vector<val> mVals;
    };

    struct sp_quote {
        val mVal;
    };

    struct sp_lambda {
        std::vector<symbol> mArgs;
        std::vector<val> mForms;
    };

    struct sp_define {
        symbol mSym;
        val mVal;
    };

    struct sp_or {
        std::vector<val> mVals;
    };

    struct sp_and {
        std::vector<val> mVals;
    };

    struct sp_if {
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
