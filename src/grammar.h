#ifndef GRAMMAR_H_SLEB5MGA
#define GRAMMAR_H_SLEB5MGA

#include "ast_adapted.h"
#include "ast_adapted.h"
/* #include "grammar_atoms.h" */
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#pragma GCC diagnostic ignored "-Wparentheses"
#include <iostream>
#include "demangle.h"

#include <boost/spirit/home/x3/string/detail/string_parse.hpp>


namespace grammar {
  namespace x3 = boost::spirit::x3;

  using x3::uint_;
  using x3::int_;
  using x3::double_;
  using x3::char_;
  using x3::hex;
  using x3::lit;
  using x3::lexeme;
  using x3::rule;
  using x3::alpha;
  using x3::alnum;
  using x3::bool_;
  using x3::string;
}

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
    struct let_class { };
    rule<let_class, ast::let> const let = "let";

    struct macro_class { };
    rule<macro_class, ast::macro> const macro("macro");

    struct lambda_class { };

    struct arg_class { };
    rule<arg_class, ast::arg> const arg("arg");

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

    // Strings
    struct str_class;
    rule<str_class, std::string> const str("str");
    auto const str_def = lexeme['"' > *(char_ - '"') > '"'];
    BOOST_SPIRIT_DEFINE(str);

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
    // Adapting bool parser for lisp bool
    // parses for #t / #f and true / false
    template <typename T = bool>
    struct lisp_bool_policies {
        template <typename Iterator, typename Attribute, typename CaseCompare>
        static bool parse_true_false(Iterator& first,
            Iterator const& last,
            Attribute& attr_,
            CaseCompare const& case_compare,
            char const* _id1,
            char const* _id2,
            T _val) {
            using namespace x3;
            auto is_id1
                = detail::string_parse(_id1, first, last, unused, case_compare);
            auto is_id2
                = detail::string_parse(_id2, first, last, unused, case_compare);

            if (is_id1 || is_id2) {
                traits::move_to(T(_val), attr_); // result is true
                return true;
            }
            return false;
        }

        template <typename Iterator, typename Attribute, typename CaseCompare>
        static bool parse_true(Iterator& first,
            Iterator const& last,
            Attribute& attr_,
            CaseCompare const& case_compare) {
            return parse_true_false(
                first, last, attr_, case_compare, "#t", "true", true);
        }

        template <typename Iterator, typename Attribute, typename CaseCompare>
        static bool parse_false(Iterator& first,
            Iterator const& last,
            Attribute& attr_,
            CaseCompare const& case_compare) {
            return parse_true_false(
                first, last, attr_, case_compare, "#f", "false", false);
        }
    };

    using lisp_bool_type = x3::bool_parser<bool,
        boost::spirit::char_encoding::standard,
        lisp_bool_policies<bool>>;

    lisp_bool_type const lisp_bool_ = {};

    // --------------------------------------------------------------------------------
    // Symbol

    struct symbol_class : boost::spirit::x3::annotate_on_success,
                          error_handler { };
    rule<symbol_class, ast::symbol, true> const symbol = "symbol";

    auto const echars = char_("?=_.!*+-/><$@");
    auto const symbol_def
        = as<std::string>[lexeme[(alpha | echars) >> *(alnum | echars)]];

    BOOST_SPIRIT_DEFINE(symbol);

    // nil
    auto constexpr f = []() {};
    struct nil_class;
    rule<nil_class, ast::nil> const nil("nil");
    // dummy semantic action to prevent
    // serializing to synthetict attribute
    auto const nil_def = lexeme[lit("nil")][f];
    BOOST_SPIRIT_DEFINE(nil);

    // Character
    // convert character code to char
    auto constexpr to_char = [](auto & _ctx) {
        std::string a = _attr(_ctx);

        char ch = '!';

        if (a.size() == 1) {
            ch = a[0];
        } else {
            if (a == "newline") {
                ch = '\n';
            }

            if (a == "tab") {
                ch = '\t';
            }

            if (a == "space") {
                ch = ' ';
            }

            if (a == "return") {
                ch = '\r';
            }

            if (a == "backspace") {
                ch = '\b';
            }

            if (a == "formfeed") {
                ch = '\f';
            }
        }
        _val(_ctx) = ch;
    };

    struct character_class;
    rule<character_class, char> const character("character");
    auto const character_def = lexeme[lit('\\') >> +alnum][to_char];
    BOOST_SPIRIT_DEFINE(character);

    // keyord
    struct keyword_class;
    rule<keyword_class, ast::keyword> const keyword("keyword");
    auto const keyword_def = lexeme[':' > symbol];
    BOOST_SPIRIT_DEFINE(keyword);

    // keyord

    // Type hint
    struct hint_class;
    rule<str_class, ast::hint> const hint("hint");
    auto const hint_def = lexeme['^' > symbol];
    BOOST_SPIRIT_DEFINE(hint);

    rule<vector_class, ast::vector> const vector("vector");
    rule<map_class, ast::map> const map("map");
    rule<map_class, ast::meta> const meta("meta");
    rule<set_class, ast::set> const set("set");

    // clang-format off

    // A Val
    rule<val_class, ast::val> const val("value");
    // clang-format on
    rule<lambda_class, ast::lambda> const lambda = "lambda";
    auto const lambda_def
        = '(' >> ( lit("lambda") | lit("fn")) > '[' > *symbol > ']' > val > ')';
    BOOST_SPIRIT_DEFINE(lambda);

    struct sexp_class { };
    rule<sexp_class, ast::sexp> const sexp = "sexp";
    auto const sexp_def = '(' > *val > ')';
    BOOST_SPIRIT_DEFINE(sexp);

    struct quote_class { };
    rule<quote_class, ast::sexp> const quote = "quote";

    auto const base_def = lexeme[lisp_bool_] | symbol | nil | keyword | str
        | character | double_ | sexp | vector | map;

    auto const special_def =  lambda | define | macro | let | quote;

    auto const val_def = special_def | base_def;

    struct base_class { };
    rule<base_class, ast::val> const base = "base";
    BOOST_SPIRIT_DEFINE(base);

    auto const quote_def = '(' >> &lit( "quote" )> *base >> ')';
    BOOST_SPIRIT_DEFINE(quote);

    BOOST_SPIRIT_DEFINE(val);

    auto const let_def = '(' >> lit("let") > '[' > *arg > ']' > val > ')';
    BOOST_SPIRIT_DEFINE(let);

    auto const arg_def = symbol >> val;
    BOOST_SPIRIT_DEFINE(arg);

    struct program_class { };
    rule<program_class, ast::program> const program = "program";
    auto const program_def                          = *val;
    BOOST_SPIRIT_DEFINE(program);

    // a define
    auto const define_def = '(' >> lit("def") > symbol > val > ')';
    BOOST_SPIRIT_DEFINE(define);

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
    
    // macro
    auto const macro_def = '(' >> lit("defmacro") >> symbol >> vector >> sexp >> ')';
    BOOST_SPIRIT_DEFINE(macro);
}

#endif /* end of include guard: GRAMMAR_H_SLEB5MGA */
