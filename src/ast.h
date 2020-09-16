#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <list>
#include <map>
#include <set>
#include <string>

/* #include <boost/config/warning_disable.hpp> */
#include <boost/mpl/copy.hpp>

#include <boost/mp11/mpl.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast {
    namespace x3 = boost::spirit::x3;
    using x3::forward_ast;


    struct nil {};

    struct list;
    struct val;
    struct vector;
    struct lambda;
    struct map;
    struct meta;
    struct set;
    struct define;
    /* struct fn; */

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

    struct keyword : x3::position_tagged {
        symbol mSym;
    };

    struct hint : x3::position_tagged {
        symbol mSym;
    };

    using namespace boost::mp11;

    template <typename T> using add_forward_ast = forward_ast<T>;

    // clang-format off

    using atoms = mp_list<
        boolean,
        symbol,
        std::string,
        double,
        char,
        keyword
            >;

    using composites = mp_list<
        lambda,
        list,
        vector,
        map,
        define
            >;

    // clang-format on


    using my_variant = mp_rename<
mp_append<atoms, mp_transform<add_forward_ast, composites>>,
        x3::variant>;

    struct val : my_variant, x3::position_tagged {
        using base_type::base_type;
        using base_type::operator=;
    };

    //
    struct define : x3::position_tagged {
        symbol mSym;
        val mVal;
    };

    struct vector : x3::position_tagged {
        std::vector<val> mForms;
    };

    struct binding : x3::position_tagged {
        symbol mSym;
        val mVal;
    };

    struct list : x3::position_tagged {
        std::vector<val> mForms;
    };

    struct map_entry : x3::position_tagged {
        val mKey;
        val mValue;
    };

    struct map : x3::position_tagged {
        std::list<map_entry> mHashMap;
    };

    struct meta : x3::position_tagged {
        std::list<map_entry> mHashMap;
    };

    struct set : x3::position_tagged {
        std::vector<val> mForms;
    };

    struct program : x3::position_tagged {
        std::vector<val> mForms;
    };

    struct lambda {
        std::vector<symbol> mArgs;
        val mBody;
    };

    // print function for debugging
    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil";
        return out;
    }
} // namespace ast

#endif /* end of include guard: AST_H_4GSXUIIF */
