#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <list>
#include <map>
#include <set>
#include <string>

#include "demangle.h"
#include "except.h"
#include "seq.h"
#include <boost/mp11/mpl.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <immer/map.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include "variant.h"

namespace ast {

    namespace x3 = boost::spirit::x3;
    using namespace boost::mp11;
    using x3::forward_ast;

    struct position_tagged : public x3::position_tagged {
        uint64_t mResultId;
    };

    template <typename T>
    struct dummy_compare {
        using type = T;
        friend bool operator==(T const& _lhs, T const& _rhs) {
            std::cout << demangle(_lhs) << std::endl;
            assert(false);
        }
    };

    struct nil : dummy_compare<nil> { };

    struct symbol : position_tagged {
        symbol() = default;

        symbol(uint64_t _v, uint64_t _scope = 0)
            : mId(_v)
            , mScope(_scope) {
        }

        friend bool operator==(symbol const& _lhs, symbol const& _rhs) {
            return _lhs.mId == _rhs.mId && _lhs.mScope == _rhs.mScope;
        }

        uint64_t mId;
        uint64_t mScope;
    };

    struct keyword : position_tagged {
        symbol mSym;
        friend bool operator==(keyword const& _lhs, keyword const& _rhs) {
            return _lhs.mSym == _rhs.mSym;
        }
    };

    struct hint : position_tagged, dummy_compare<hint> {
        symbol mSym;
    };

    // clang-format off
    using atoms
        = mp_list< bool
                 , std::string
                 , double
                 , char
                 , keyword
                 , nil
                 >;

    // clang-format on

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
        , symbol
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

    struct val : val_base_t, position_tagged {

        template <typename T>
        val(T&& rhs)
            : val_base_t(std::forward<T>(rhs)) {
        }

        friend bool operator==(val const& _lhs, val const& _rhs) {
            return _lhs.var == _rhs.var;
        }

        val();

        val(val const& _v) = default;

        val& operator=(val const& _rhs) {
            this->var = _rhs.var;
            return *this;
        }

        bool to_bool() const;
        bool is_atom() const;
    };


    struct native_function : val::callable_t {

        using native_f_t
            = std::function<val(Evaluator&, iterator_base_t& _args)>;

        native_f_t mFunc;

        int mNumOfArgs;

        friend bool operator==(
            native_function const& _lhs, native_function const& _rhs);

        val call(Evaluator& _e, iterator_base_t& _args) const;
    };

    struct recur : dummy_compare<recur> {
        std::vector<val> mArgs;
    };

}

namespace ast {
    struct map_entry : position_tagged {
        val mKey;
        val mValue;
        friend bool operator==(map_entry const& _lhs, map_entry const& _rhs);
    };

    struct map : position_tagged {
        std::list<map_entry> mHashMap;
        val get(ast::val const& _key) const;
        void add(val const& _key, val const& _val);

        friend bool operator==(map const& _lhs, map const& _rhs);
    };

    struct set : position_tagged, dummy_compare<set> {
        std::vector<val> mForms;
        friend bool operator==(set const& _lhs, set const& _rhs);
    };

    struct vector : position_tagged, seq_t {
        vector() = default;
        virtual std::unique_ptr<iterator_base_t> iterator() const;
        vector(std::vector<val> const& _init);

        std::vector<val> mForms;
        friend bool operator==(vector const& _lhs, vector const& _rhs);
    };

    struct sexp : position_tagged, dummy_compare<sexp>, seq_t {
        sexp() = default;

        sexp(std::vector<val> const& _init);

        sexp(std::unique_ptr<iterator_base_t>& _ptr) {
            while (auto e = _ptr->next()) {
                mForms.push_back(*e);
            }
        }

        virtual std::unique_ptr<iterator_base_t> iterator() const;
        void conj(ast::val const& _val);
        bool is(char const* _symName);

        std::vector<val> mForms;
    };

    struct program : position_tagged, seq_t, dummy_compare<program> {
        std::vector<val> mForms;
        virtual std::unique_ptr<iterator_base_t> iterator() const;
    };
}

namespace ast {

    struct meta : position_tagged {
        std::list<map_entry> mHashMap;
        friend bool operator==(meta const& _lhs, meta const& _rhs);
    };

    struct args : position_tagged, dummy_compare<args> {
        vector mArgs;
        std::optional<symbol> mExtra;
    };

    struct define : position_tagged, dummy_compare<define> {
        define() = default;
        define(sexp const& exp);

        symbol mSym;
        val mVal;
    };

    struct pair : position_tagged, dummy_compare<pair> {
        val mFirst;
        val mSecond;
    };

    struct bindings : position_tagged, dummy_compare<bindings> {
        std::vector<pair> mBindings;
    };

    struct let : position_tagged, dummy_compare<let> {
        bindings mBindings;
        program mBody;
    };

    struct macro : position_tagged, dummy_compare<macro> {
        symbol mSym;
        args mArgs;
        val mVal;
    };

    // print function for debugging
    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil";
        return out;
    }

    struct lambda : position_tagged, dummy_compare<lambda>, val::callable_t {
        args mArgs;
        boost::optional<std::string> mDocString;
        program mBody;
        /* friend bool operator==(lambda const& _lhs, lambda const& _rhs); */

        virtual val call(Evaluator& _e, iterator_base_t&) const {
            assert(false);
        }
    };

} // namespace ast

#endif /* end of include guard: AST_H_4GSXUIIF */
