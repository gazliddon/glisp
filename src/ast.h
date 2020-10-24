#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <list>
#include <map>
#include <set>
#include <string>

#include "except.h"
#include <boost/mp11/mpl.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <immer/map.hpp>
#include <spdlog/spdlog.h>

namespace ast {
    template <typename T>
    struct dummy_compare {
        using type = T;
        friend bool operator==(T const& _lhs, T const& _rhs) {
            assert(false);
        }
    };
}

namespace ast {
    namespace x3 = boost::spirit::x3;
    using namespace boost::mp11;

    struct v4 { };

    struct nil {
        friend bool operator==(nil const& _lhs, nil const& _rhs) {
            return true;
        }
    };

    struct symbol : x3::position_tagged {
        symbol() {
        }
        symbol(char const* _name)
            : mName(_name) {
        }
        symbol(std::string const& _name)
            : mName(_name) {
        }

        std::string mName;
        friend bool operator==(symbol const& _lhs, symbol const& _rhs);
    };

    struct resolved_symbol_t : dummy_compare<resolved_symbol_t> {
        resolved_symbol_t() {}
        resolved_symbol_t(uint64_t _v) : mSymbolId(_v) {
        }
        uint64_t mSymbolId;
    };

    struct keyword : x3::position_tagged {

        symbol mSym;

        friend bool operator==(keyword const& _lhs, keyword const& _rhs);
    };

    struct hint : x3::position_tagged {
        symbol mSym;
    };

    using atoms = mp_list<bool, std::string, double, char, keyword, nil, resolved_symbol_t>;

    template <typename T>
    constexpr bool is_atom() {
        return mp_count<atoms, T>() != 0;
    }

}

namespace ast {
    using namespace boost::mp11;
    namespace x3 = boost::spirit::x3;
    using x3::forward_ast;

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
    struct arg;
    struct let;
    /* struct fn; */

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
        , arg
        , let
        >;

    // clang-format on

    template <typename T>
    using add_forward_ast = forward_ast<T>;

    using my_variant
        = mp_rename<mp_append<atoms, mp_transform<add_forward_ast, composites>>,
            x3::variant>;

    struct val : my_variant, x3::position_tagged {
        using base_type::base_type;
        using base_type::operator=;
        friend bool operator==(val const& _lhs, val const& _rhs) {
            return _lhs.var == _rhs.var;
        }

        val() {
            *this = ast::nil();
        }

        val(val const&) = default;

        val& operator=(val const&) = default;

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
        T * get_val() {

            if (is<T>()) {
                if constexpr (mp_count<atoms, T>() != 0) {
                    return boost::get<T>(this);
                } else {
                    return boost::get<forward_ast<T>>(this)->get_pointer();
                }
            }
            return nullptr;
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

    struct Evaluator;

    struct native_function {
        std::function<val(Evaluator&, std::vector<val> const&)> mFunc;
        int mNumOfArgs;
        friend bool operator==(
            native_function const& _lhs, native_function const& _rhs);

        val call(Evaluator& _e,
            std::string const& _binding,
            std::vector<val> const& args) const {
            if (args.size() < unsigned(mNumOfArgs)) {
                auto x = fmt::format("Too many args for native function "
                                     "{}\nExpected at least {}, got {}",
                    _binding,
                    mNumOfArgs,
                    args.size());

                throw(glisp::cEvalError(x.c_str()));
            }
            return mFunc(_e, args);
        }
    };

    struct vector : x3::position_tagged {
        vector() {
        }

        vector(std::vector<val> const& _init)
            : mForms(_init) {
        }

        std::vector<val> mForms;
        friend bool operator==(vector const& _lhs, vector const& _rhs);
    };

    struct map_entry : x3::position_tagged {
        val mKey;
        val mValue;
        friend bool operator==(map_entry const& _lhs, map_entry const& _rhs);
    };

    struct map : x3::position_tagged {
        std::list<map_entry> mHashMap;

        val get(ast::val const& _key) const {
            auto& hmap = mHashMap;
            for (auto i = hmap.begin(); i != hmap.end(); i++) {
                auto this_key = i->mKey;
                if (this_key == _key) {
                    return i->mValue;
                }
            }
            return val();
        }

        void add(val const& _key, val const& _val) {
            auto m = map_entry { .mKey = _key, .mValue = _val };
            mHashMap.push_back(m);
        }

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
        friend bool operator==(program const& _lhs, program const& _rhs);
    };

    struct lambda {
        std::vector<symbol> mArgs;
        boost::optional<symbol> mFinalArg;
        boost::optional<std::string> mDocString;
        program mBody;
        friend bool operator==(lambda const& _lhs, lambda const& _rhs);
    };


    struct recur : dummy_compare<recur> {
        std::vector<val> mArgs;
    };

    struct sexp : dummy_compare<sexp> {
        sexp() {
        }

        sexp(std::vector<val> const& _init)
            : mForms(_init) {
        }
        std::vector<val> mForms;

        void conj(ast::val const& _val) {
            mForms.push_back(_val);
        }

        bool is(char const* _symName) {
            return false;
        }
    };

    struct define : x3::position_tagged, dummy_compare<define> {
        define() {
        }

        define(sexp const& exp) {
            assert(exp.mForms.size() == 3);
            auto define_id = exp.mForms[0].get_val<symbol>();
            assert(define_id);
            assert(define_id->mName == std::string("define"));
            auto sym_id = exp.mForms[1].get_val<symbol>();
            assert(sym_id);
            mSym = *sym_id;
            mVal = exp.mForms[2];
        }

        symbol mSym;
        val mVal;
    };

    struct arg : x3::position_tagged, dummy_compare<arg> {
        symbol mSymbol;
        val mVal;
    };

    struct let : x3::position_tagged, dummy_compare<let> {
        std::vector<arg> mArgs;
        program mBody;
    };

    struct macro : x3::position_tagged, dummy_compare<macro> {
        symbol mSym;
        vector mArgs;
        val mVal;
    };

    // print function for debugging
    inline std::ostream& operator<<(std::ostream& out, nil) {
        out << "nil";
        return out;
    }

} // namespace ast

#endif /* end of include guard: AST_H_4GSXUIIF */
