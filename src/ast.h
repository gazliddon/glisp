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
#include <immer/map.hpp>

namespace ast { }


namespace ast {
    namespace x3 = boost::spirit::x3;
    using namespace boost::mp11;

    struct nil { 
        friend bool operator==(nil const& _lhs, nil const& _rhs) {
            return true;
        }
    };

    struct symbol : x3::position_tagged {
        std::string mName;
        friend bool operator==(symbol const& _lhs, symbol const& _rhs);
    };

    struct keyword : x3::position_tagged {
        symbol mSym;
        friend bool operator==(keyword const& _lhs, keyword const& _rhs);
    };

    struct hint : x3::position_tagged {
        symbol mSym;
    };

    using atoms = mp_list<bool, std::string, double, char, keyword, nil>;

    template <typename T>
    constexpr bool is_atom() {
        return mp_count<atoms, T>() != 0;
    }

}

namespace ast {
    using namespace boost::mp11;
    namespace x3 = boost::spirit::x3;
    using x3::forward_ast;


    struct list;
    struct val;
    struct vector;
    struct lambda;
    struct map;
    struct meta;
    struct set;
    struct define;
    struct function;
    struct native_function;
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
        , function
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

        bool to_bool() const {
            if (is<nil>()) {
                return false;
            }

            if (is<bool>()) {
                return *get_val<bool>();
            }

            return true;
        }

        template <typename T>
        bool is() const {
            auto id = var.which();
            return id == mp_find<types, T>();
        }
        bool is_atom() const {
            auto i = var.which();
            return i < int(mp_size<atoms>());
        }

        template <typename T>
        T const* get_val() const {

            if (is<T>()) {
                if constexpr (mp_count<atoms, T>() != 0) {
                    return boost::get<T>(this);
                } else {
                    return boost::get<forward_ast<T>>(this)->get_pointer();
                }
            }
            return nullptr;
        }
    };

    using env_t = immer::map<std::string, val>;

    struct native_function {
        std::function<val(env_t, std::vector<val> const&)> mFunc;
        int mNumOfArgs;
        friend bool operator==(
            native_function const& _lhs, native_function const& _rhs);
    };

    struct define : x3::position_tagged {
        symbol mSym;
        val mVal;
        friend bool operator==(define const& _lhs, define const& _rhs);
    };

    struct vector : x3::position_tagged {
        std::vector<val> mForms;
        friend bool operator==(vector const& _lhs, vector const& _rhs);
    };

    struct list : x3::position_tagged {
        std::vector<val> mForms;
        friend bool operator==(list const& _lhs, list const& _rhs);
    };

    struct map_entry : x3::position_tagged {
        val mKey;
        val mValue;
        friend bool operator==(map_entry const& _lhs, map_entry const& _rhs);
    };

    struct map : x3::position_tagged {
        std::list<map_entry> mHashMap;
        friend bool operator==(map const& _lhs, map const& _rhs);
    };

    struct meta : x3::position_tagged {
        std::list<map_entry> mHashMap;
        friend bool operator==(meta const& _lhs, meta const& _rhs);
    };

    struct set : x3::position_tagged {
        std::vector<val> mForms;
        friend bool operator==(set const& _lhs, set const& _rhs);
    };

    struct program : x3::position_tagged {
        std::vector<val> mForms;
    };

    struct lambda {
        std::vector<symbol> mArgs;
        val mBody;
        friend bool operator==(lambda const& _lhs, lambda const& _rhs);
    };

    struct function {
        val mFunc;
        std::vector<val> mArgs;
        friend bool operator==(function const& _lhs, function const& _rhs);
    };

    // print function for debugging
    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil";
        return out;
    }
} // namespace ast

#endif /* end of include guard: AST_H_4GSXUIIF */
