#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <list>
#include <map>
#include <set>
#include <string>

#include "demangle.h"
#include "except.h"
#include "variant.h"
#include <boost/mp11/mpl.hpp>
#include <boost/spirit/home/x3.hpp>
#include <immer/map.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

#include "position_tag_t.h"
#include "symbol_t.h"
#include "typed_number_t.h"

namespace ast {

    namespace x3 = boost::spirit::x3;
    using namespace boost::mp11;
    using x3::forward_ast;

    template <typename T>
    struct dummy_compare {
        using type = T;
        friend bool operator==(T const& _lhs, T const& _rhs) {
            std::cout << demangle(_lhs) << std::endl;
            assert(false);
        }
    };


    struct nil : dummy_compare<nil> { };

    struct keyword : position_tagged_t {
        symbol_t mSym;
        friend bool operator==(keyword const& _lhs, keyword const& _rhs) {
            return _lhs.mSym == _rhs.mSym;
        }
    };

    struct hint : position_tagged_t, dummy_compare<hint> {
        symbol_t mSym;
    };

    struct unbound : dummy_compare<unbound> { };

    // clang-format off
    using non_number_atoms = mp_list <
                  std::string
                 , char
                 , keyword
                 , nil
                 , unbound
                 , bool
                 , typed_number_t
        >;
    // clang-format on

    using atoms = mp_append<number_types, non_number_atoms>;

    template <typename T>
    constexpr bool is_atom() {
        return mp_count<atoms, T>() != 0;
    }

    struct val;
    struct vector;
    struct lambda;
    struct map;
    struct meta;
    struct set;
    struct define;
    struct sexp;
    struct native_function;
    struct program;
    struct macro;
    struct bindings;
    struct args;
    struct let;
    struct pair;

    // clang-format off

    using composites = mp_list
        < lambda
        , vector
        , map
        , define
        , symbol_t
        , native_function
        , sexp
        , program
        , macro
        , bindings
        , args
        , let
        , pair
        >;

    using all_types
        = mp_append<atoms, mp_transform<add_forward_ast, composites>>;

    using val_base_t = mp_rename<all_types, variant_base_t>;

    struct val : public val_base_t, position_tagged_t {
        using base_type = val_base_t;
        using base_type::getVar;

        struct callable_t {
            virtual val call(Evaluator& _e, cIterator &) const = 0;
            virtual ~callable_t() {
            }
        };

        template <typename T>
        val(T&& rhs)
            : val_base_t(std::forward<T>(rhs)) {
        }

        val();

        val(val const& _v) = default;

        val& operator=(val const& _rhs) {
            getVar() = _rhs.getVar();
            return *this;
        }

        bool to_bool() const;
        bool is_atom() const;

        std::string typeName() const;

        friend bool operator==(val const& _lhs, val const& _rhs) {
            return _lhs.getVar() == _rhs.getVar();
        }
    };

    class cIterator;

    struct native_function : val::callable_t {

        using native_f_t
            = std::function<val(Evaluator&, cIterator & _args)>;

        native_f_t mFunc;

        int mNumOfArgs;

        friend bool operator==(
            native_function const& _lhs, native_function const& _rhs);

        val call(Evaluator& _e, cIterator & _args) const;
    };

    struct recur : dummy_compare<recur> {
        std::vector<val> mArgs;
    };

}

namespace ast {
    struct map_entry : position_tagged_t {
        val mKey;
        val mValue;
        friend bool operator==(map_entry const& _lhs, map_entry const& _rhs);
    };

    struct map : position_tagged_t {
        std::list<map_entry> mHashMap;
        val get(ast::val const& _key) const;
        void add(val const& _key, val const& _val);


        friend bool operator==(map const& _lhs, map const& _rhs);
    };

    struct set : position_tagged_t, dummy_compare<set> {
        std::vector<val> mForms;
        friend bool operator==(set const& _lhs, set const& _rhs);
    };

    struct vector : position_tagged_t {
        vector() = default;
        vector(std::vector<val> const& _init);

        std::vector<val> mForms;
        friend bool operator==(vector const& _lhs, vector const& _rhs);
    };

    struct sexp : position_tagged_t, dummy_compare<sexp> {
        sexp() = default;

        sexp(std::vector<val> const& _init);
        sexp(cIterator & _ptr);

        void conj(ast::val const& _val);
        bool is(char const* _symName);

        std::vector<val> mForms;
    };

    struct program : position_tagged_t, dummy_compare<program> {
        std::vector<val> mForms;
    };
}

namespace ast {

    struct meta : position_tagged_t {
        std::list<map_entry> mHashMap;
        friend bool operator==(meta const& _lhs, meta const& _rhs);
    };

    struct args : position_tagged_t, dummy_compare<args> {
        vector mArgs;
        std::optional<symbol_t> mExtra;
    };

    struct define : position_tagged_t, dummy_compare<define> {
        define() = default;
        define(sexp const& exp);

        symbol_t mSym;
        val mVal;
    };

    struct pair : position_tagged_t, dummy_compare<pair> {
        val mFirst;
        val mSecond;
    };

    struct bindings : position_tagged_t, dummy_compare<bindings> {
        std::vector<val> mBindings;
    };

    struct let : position_tagged_t, dummy_compare<let> {
        bindings mBindings;
        program mBody;
    };

    struct macro : position_tagged_t, dummy_compare<macro> {
        symbol_t mSym;
        args mArgs;
        val mVal;
    };

    // print function for debugging
    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil";
        return out;
    }

    struct lambda : position_tagged_t, dummy_compare<lambda>, val::callable_t {
        args mArgs;
        boost::optional<std::string> mDocString;
        program mBody;
        /* friend bool operator==(lambda const& _lhs, lambda const& _rhs); */

        virtual val call(Evaluator& _e, cIterator &) const {
            assert(false);
        }
    };

} // namespace ast

#endif /* end of include guard: AST_H_4GSXUIIF */
