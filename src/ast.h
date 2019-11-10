#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <list>
#include <map>
#include <set>
#include <string>

/* #include <boost/config/warning_disable.hpp> */
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

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
    struct sp_let;

    struct sp_null : x3::position_tagged {};

    struct symbol : x3::position_tagged {
        char mStart;
        std::string mName;
        std::string get() const {
            return mStart + mName;
        }
    };

    struct boolean : x3::position_tagged {
        std::string mVal;
    };

    struct keyword : x3::position_tagged{
        symbol mSym;
    };

    struct hint : x3::position_tagged {
        symbol mSym;
    };


    // clang-format off

    struct val : x3::variant<
                 boolean
                 , symbol
                 , std::string
                 , double
                 , char
                 , keyword
                 , sp_null
                 , forward_ast<application>
                 , forward_ast<sp_define>
                 , forward_ast<sp_if>
                 , forward_ast<sp_or>
                 , forward_ast<sp_and>
                 , forward_ast<sp_lambda>
                 , forward_ast<sp_quote>
                 , forward_ast<sp_list>
                 , forward_ast<vector>
                 , forward_ast<map>
                 , forward_ast<sp_let>
                  > , x3::position_tagged {

        using base_type::base_type;
        using base_type::operator=;
    };

    // clang-format on
    //
    
    struct vector : x3::position_tagged {
        std::vector<val> mForms;
    };

    struct binding : x3::position_tagged{
        symbol mSym;
        val mVal;
    };

    struct sp_let : x3::position_tagged{
        std::vector<binding> mBindings;
        val mBody;
    };

    struct sp_list : x3::position_tagged{
        std::vector<val> mVals;
    };

    struct sp_quote : x3::position_tagged{
        val mVal;
    };

    struct sp_lambda : x3::position_tagged{
        vector mArgs;
        std::vector<val> mForms;
    };

    struct sp_define : x3::position_tagged{
        symbol mSym;
        val mVal;
    };

    struct sp_or : x3::position_tagged {
        std::vector<val> mVals;
    };

    struct sp_and : x3::position_tagged {
        std::vector<val> mVals;
    };

    struct sp_if : x3::position_tagged {
        val mPred, mTrue, mFalse;
    };

    struct application : x3::position_tagged{
        val mFunc;
        std::vector<val> mForms;
    };


    struct map_entry : x3::position_tagged{
        val mKey;
        val mValue;
    };

    struct map : x3::position_tagged{
        std::list<map_entry> mHashMap;
    };

    struct meta : x3::position_tagged{
        std::list<map_entry> mHashMap;
    };

    struct set : x3::position_tagged{
        std::vector<val> mForms;
    };

    struct program : x3::position_tagged {
        std::vector<val> mForms;
    };

    // print function for debugging
    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil";
        return out;
    }
} // namespace ast

#endif /* end of include guard: AST_H_4GSXUIIF */
