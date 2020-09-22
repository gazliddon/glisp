#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <list>
#include <map>
#include <set>
#include <string>

#include <boost/mp11/mpl.hpp>
#include <boost/mpl/copy.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast {
    struct env_t;
}

namespace ast {
    using namespace boost::mp11;

    namespace x3 = boost::spirit::x3;

    struct boolean : x3::position_tagged {
        std::string mVal;
    };

    struct symbol : x3::position_tagged {
        char mStart;
        std::string mName;
        std::string get() const {
            return mStart + mName;
        }
    };

    struct keyword : x3::position_tagged {
        symbol mSym;
    };

    struct hint : x3::position_tagged {
        symbol mSym;
    };

    using atoms = mp_list<boolean, std::string, double, char, keyword>;

    template <typename T>
    constexpr bool is_atom() {
        return mp_count<atoms, T>() != 0;
    }

    struct val;
    struct env_t;
    struct native_function {
        /* std::function<val(env_t const &, val const &, int )> mFunc; */
        std::function<void(void)> mFunc;
        int mNumOfArgs;
    };

}

namespace ast {
    using namespace boost::mp11;
    namespace x3 = boost::spirit::x3;
    using x3::forward_ast;

    struct nil { };

    struct list;
    struct val;
    struct vector;
    struct lambda;
    struct map;
    struct meta;
    struct set;
    struct define;
    struct function_t;
    /* struct fn; */

    // clang-format off

    using composites = mp_list
        < lambda
        , list
        , vector
        , map
        , define
        , symbol
        , native_function
        , function_t
        >;

    // clang-format on
    //

    template <typename T>
    using add_forward_ast = forward_ast<T>;

    using my_variant
        = mp_rename<mp_append<atoms, mp_transform<add_forward_ast, composites>>,
            x3::variant>;

    struct val : my_variant, x3::position_tagged {
        using base_type::base_type;
        using base_type::operator=;

        template <typename T>
        bool is() const {
            auto id = var.which();
            return id == mp_find<types, T>();
        }

        template <typename T>
        T* get_val() {
            if constexpr (is_atom<T>()) {
                return boost::get<T>(this);
            }
            if constexpr (mp_count<composites, T>() != 0) {
                return boost::get<forward_ast<T>>(this)->get_pointer();
            }

            return nullptr;
        }

        bool is_atom() const {
            auto i = var.which();
            return i < int(mp_size<atoms>());
        }
    };

    struct define : x3::position_tagged {
        symbol mSym;
        val mVal;
    };

    struct vector : x3::position_tagged {
        std::vector<val> mForms;
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


    struct function : x3::variant<forward_ast<function_t>, symbol, lambda> {
        template <typename T>
        bool is() const {
            auto id = var.which();
            return id == mp_find<types, T>();
        }
    };

    struct function_t {
        function mFunc;
        std::vector<val> mArgs;
    };

    // print function for debugging
    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil";
        return out;
    }
} // namespace ast

#endif /* end of include guard: AST_H_4GSXUIIF */
