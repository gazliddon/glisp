#ifndef GRAMMAR_H_SLEB5MGA
#define GRAMMAR_H_SLEB5MGA

#include "ast_adapted.h"
#include "grammar_atoms.h"
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#pragma GCC diagnostic ignored "-Wparentheses"
#include <iostream>

#include <cxxabi.h>

namespace grammar {

    struct error_handler {
        template <typename Iterator, typename Exception, typename Context>
        boost::spirit::x3::error_handler_result on_error(Iterator& first,
            Iterator const& last,
            Exception const& x,
            Context const& context) {
            auto& error_handler
                = boost::spirit::x3::get<boost::spirit::x3::error_handler_tag>(
                    context)
                      .get();
            std::string message
                = "GAZ Error! Expecting: " + x.which() + " here:";
            error_handler(x.where(), message);
            return boost::spirit::x3::error_handler_result::fail;
        }
    };
    namespace x3 = boost::spirit::x3;

    using x3::alnum;
    using x3::alpha;
    using x3::lexeme;
    using x3::no_skip;
    using x3::repeat;
    using x3::rule;
    using x3::space;
    /* using x3::space; */
    /* using x3::string; */

    // composite types

    struct lambda_class { };
    struct list_class : boost::spirit::x3::annotate_on_success,
                        error_handler { };
    /* struct vector_class : boost::spirit::x3::annotate_on_success,
     * error_handler {};; */
    struct vector_class { };
    struct map_entry_class : boost::spirit::x3::annotate_on_success,
                             error_handler { };
    ;
    struct map_class : boost::spirit::x3::annotate_on_success,
                       error_handler { };
    ;
    struct meta_class : boost::spirit::x3::annotate_on_success,
                        error_handler { };
    ;
    struct set_class : boost::spirit::x3::annotate_on_success,
                       error_handler { };
    ;
    struct val_class : boost::spirit::x3::annotate_on_success,
                       error_handler { };

    auto const ws = no_skip[+lit(" ")];

    auto const term = no_skip[ws | '(' | ')'];

    struct define_class { };
    rule<define_class, ast::define> const define = "define";

    // bool
    struct boolean_class;
    rule<boolean_class, ast::boolean> const boolean("boolean");
    auto const boolean_def = (string("#t") | string("#f"));
    BOOST_SPIRIT_DEFINE(boolean);

    // Strings
    struct str_class;
    rule<str_class, std::string> const str("str");
    auto const str_def = lexeme['"' > *(char_ - '"') > '"'];
    BOOST_SPIRIT_DEFINE(str);

    template <typename T>
    std::string demangle() {
        const size_t max_size = 20000;

        char buff[max_size];
        auto mangled = typeid(T).name();
        int stat     = 0;
        size_t len   = max_size;

        auto c = abi::__cxa_demangle(mangled, buff, &len, &stat);
        return c;
    }

    template <typename T>
    std::string demangle(T const& _arg) {
        return demangle<decltype(_arg)>();
    }

    // --------------------------------------------------------------------------------
    // Helpful adaptor
    template <typename T>
    struct as_type {
        template <typename E>
        constexpr auto operator[](E e) const {
            return x3::rule<struct _, T> {} = e;
        }
    };

    template <typename T>
    static inline constexpr as_type<T> as;

    // --------------------------------------------------------------------------------
    // Symbol


    struct symbol_class : boost::spirit::x3::annotate_on_success,
                          error_handler { };
    rule<symbol_class, ast::symbol, true> const symbol = "symbol";

    auto const echars = char_("=_.?!*+-/><$@");

    auto const symbol_def
        = as<std::string> [lexeme[(alpha | echars) >> *(alnum | echars)]];

    BOOST_SPIRIT_DEFINE(symbol);

    // Character
    struct character_class;
    rule<character_class, char> const character("character");
    auto const character_def = lexeme[lit('\\') >> char_];
    BOOST_SPIRIT_DEFINE(character);

    // keyord
    struct keyword_class;
    rule<keyword_class, ast::keyword> const keyword("keyword");
    auto const keyword_def = lexeme[':' > symbol];
    BOOST_SPIRIT_DEFINE(keyword);

    // Type hint
    struct hint_class;
    rule<str_class, ast::hint> const hint("hint");
    auto const hint_def = lexeme['^' > symbol];
    BOOST_SPIRIT_DEFINE(hint);

    rule<vector_class, ast::vector> const vector("vector");
    rule<map_class, ast::map> const map("map");
    rule<map_class, ast::meta> const meta("meta");
    rule<set_class, ast::set> const set("set");
    rule<list_class, ast::list> const list("list");

    // clang-format off

    // A Val
    rule<val_class, ast::val> const val("value");
    // clang-format on
    rule<lambda_class, ast::lambda> const lambda = "lambda";
    auto const lambda_def
        = '(' >> lit("lambda") > '[' > *symbol > ']' > val > ')';
    BOOST_SPIRIT_DEFINE(lambda);

    struct function_class { };
    rule<function_class, ast::function> const function = "function";
    auto const function_def = '(' > (symbol | lambda | function) > *val > ')';
    BOOST_SPIRIT_DEFINE(function);

    auto const val_def = boolean | symbol | keyword | str | character | double_
        | lambda | define | function | list | vector | map;

    BOOST_SPIRIT_DEFINE(val);

    struct program_class { };
    rule<program_class, ast::program> const program = "program";
    auto const program_def                          = *val;
    BOOST_SPIRIT_DEFINE(program);

    // a define
    auto const define_def = '(' >> lit("define") > symbol > val > ')';
    BOOST_SPIRIT_DEFINE(define);

    // List
    auto const list_def = '(' > *val > ')';
    BOOST_SPIRIT_DEFINE(list);

    // A vector
    auto const vector_def = '[' > *val > ']';
    BOOST_SPIRIT_DEFINE(vector);

    // A set
    auto const set_def = "#{" > *val > '}';
    BOOST_SPIRIT_DEFINE(set);

    // a map entry
    rule<map_entry_class, ast::map_entry> const map_entry("map_entry");
    auto const map_entry_def = val > val;
    BOOST_SPIRIT_DEFINE(map_entry);

    // a map
    auto const map_def = '{' > *map_entry > '}';
    BOOST_SPIRIT_DEFINE(map);

    // meta data
    auto const meta_def = lit("^{") > *map_entry > '}';
    BOOST_SPIRIT_DEFINE(meta);
}

#endif /* end of include guard: GRAMMAR_H_SLEB5MGA */
