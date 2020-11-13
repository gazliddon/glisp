#ifndef GRAMMAR_H_SLEB5MGA
#define GRAMMAR_H_SLEB5MGA

#include "ast_adapted.h"
#include "symtab.h"

/* #include "grammar_atoms.h" */
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#pragma GCC diagnostic ignored "-Wparentheses"
#include "demangle.h"
#include <iostream>

#include <boost/spirit/home/x3/string/detail/string_parse.hpp>

namespace grammar {
    namespace x3 = boost::spirit::x3;
    struct position_cache_tag { };

    struct annotate_position {
        template <typename T, typename Iterator, typename Context>
        inline void on_success(Iterator const& first,
            Iterator const& last,
            T& ast,
            Context const& context) {
            auto& position_cache = x3::get<position_cache_tag>(context).get();
            position_cache.annotate(ast, first, last);
        }
    };

    using x3::alnum;
    using x3::alpha;
    using x3::bool_;
    using x3::char_;
    using x3::double_;
    using x3::hex;
    using x3::int_;
    using x3::lexeme;
    using x3::lit;
    using x3::no_skip;
    using x3::repeat;
    using x3::rule;
    using x3::space;
    using x3::string;
    using x3::uint_;

    struct error_handler {
        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(Iterator& first,
            Iterator const& last,
            Exception const& x,
            Context const& context) {
            auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
            std::string message = "Error! Expecting: " + x.which() + " here:";
            error_handler(x.where(), message);
            return x3::error_handler_result::fail;
        }
    };

    /* using x3::space; */
    /* using x3::string; */

    // composite types
    struct let_class : x3::annotate_on_success { };
    rule<let_class, ast::let, true> const let = "let";

    struct macro_class : x3::annotate_on_success { };
    rule<macro_class, ast::macro> const macro("macro");

    struct lambda_class : x3::annotate_on_success { };

    struct vector_class : x3::annotate_on_success { };
    struct map_entry_class : x3::annotate_on_success { };
    struct map_class : x3::annotate_on_success { };
    struct meta_class : x3::annotate_on_success { };
    struct set_class : x3::annotate_on_success { };
    struct val_class : x3::annotate_on_success { };
    struct args_class : x3::annotate_on_success { };

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
            return x3::rule<struct _, T, true> {} = e;
        }
    };

    template <typename T>
    static inline constexpr as_type<T> as;

    auto constexpr set_bool_true = [](auto& _ctx) { _val(_ctx) = true; };
    struct is_true_class { };
    rule<is_true_class, bool> const is_true("true");
    auto const is_true_def = lit("true")[set_bool_true];
    BOOST_SPIRIT_DEFINE(is_true);

    auto constexpr set_bool_false = [](auto& _ctx) { _val(_ctx) = false; };
    struct is_false_class { };
    rule<is_false_class, bool> const is_false("false");
    auto const is_false_def = lit("false")[set_bool_false];
    BOOST_SPIRIT_DEFINE(is_false);

    auto ctx_info = [](auto& ctx) {
        auto& attr = _attr(ctx);
        auto& val  = _val(ctx);

        using namespace std;
        cout << "attr type: " << demangle(attr) << endl;
        cout << "val type: " << demangle(val) << endl;
    };

    // --------------------------------------------------------------------------------
    // Symbol

    struct symbol_class : x3::annotate_on_success { };
    rule<symbol_class, ast::symbol> const symbol = "symbol";

    auto get_symbol = [](auto& ctx, std::string const& _name) -> ast::symbol {
        auto& symtab = x3::get<ast::cSymTab>(ctx).get();
        auto id      = symtab.getIdOrRegister(_name);
        return { id };
    };

    auto process_symbol
        = [](auto& ctx) { _val(ctx) = get_symbol(ctx, _attr(ctx)); };

    auto const echars = char_("?=_.!*+-/><$@");
    auto const get_def
        = as<std::string>[lexeme[(alpha | echars) >> *(alnum | echars | '-')]];
    auto const symbol_def = get_def[process_symbol];

    BOOST_SPIRIT_DEFINE(symbol);

    // --------------------------------------------------------------------------------
    // nil
    auto constexpr f = []() {};
    struct nil_class;
    rule<nil_class, ast::nil> const nil("nil");

    // --------------------------------------------------------------------------------
    // Character
    // convert character code to char
    auto constexpr to_char = [](auto& _ctx) {
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
    struct keyword_class : x3::annotate_on_success { };
    rule<keyword_class, ast::keyword> const keyword("keyword");
    auto const keyword_def = lexeme[':' > symbol];
    BOOST_SPIRIT_DEFINE(keyword);

    // keyord

    // Type hint
    struct hint_class : x3::annotate_on_success { };
    rule<hint_class, ast::hint> const hint("hint");
    auto const hint_def = lexeme['^' > symbol];
    BOOST_SPIRIT_DEFINE(hint);

    rule<vector_class, ast::vector> const vector("vector");
    rule<map_class, ast::map> const map("map");
    rule<map_class, ast::meta> const meta("meta");
    rule<args_class, ast::args> const args("args");
    rule<set_class, ast::set> const set("set");
    rule<val_class, ast::val> const val("value");
    rule<lambda_class, ast::lambda> const lambda = "lambda";

    struct program_class : error_handler { };
    rule<program_class, ast::program> const program = "program";

    // A Val
    auto const lambda_def = '(' >> (lit("fn")) > -(str) > args > program > ')';
    BOOST_SPIRIT_DEFINE(lambda);

    struct sexp_class : x3::annotate_on_success { };
    rule<sexp_class, ast::sexp> const sexp = "sexp";
    auto const sexp_def                    = '(' > *val > ')';
    BOOST_SPIRIT_DEFINE(sexp);

    struct quote_class : x3::annotate_on_success { };
    rule<quote_class, ast::sexp> const quote = "quote";

    struct tester_class;
    rule<tester_class, std::string> const tester("tester");

    struct quoted_class : x3::annotate_on_success { };
    rule<quoted_class, ast::sexp> const quoted = "quoted";

    // clang-format off
    auto const base_def = 
          is_true
        | is_false
        | symbol 
        | nil
        | keyword
        | str
        | character
        | double_
        | sexp
        | vector
        | map;

    auto const special_def = 
          quote
        /* | define */
        | let
        | lambda;

    auto const val_def =
          quoted
        | special_def
        | base_def;

    // clang-format one

    struct base_class { };
    rule<base_class, ast::val> const base = "base";
    BOOST_SPIRIT_DEFINE(base);

    auto const quote_def = '(' >> &lit( "quote" )> *base >> ')';
    BOOST_SPIRIT_DEFINE(quote);

    auto constexpr quoted_fn =  [](auto & _ctx) {

        auto& symtab = x3::get<ast::cSymTab>(_ctx).get();

        ast::val & val = _attr(_ctx);
        ast::sexp & ret = _val(_ctx);

        auto sym = ast::symbol(symtab.getIdOrRegister("quote"));

        ret.mForms.push_back(ast::val(sym));
        ret.mForms.push_back(val);
        ret.id_first = val.id_first;
        ret.id_last = val.id_last;
    };
    
    // dummy semantic action to prevent
    // serializing to synthetict attribute
    auto const nil_def = lexeme[lit("nil")][f];
    BOOST_SPIRIT_DEFINE(nil);

    auto const quoted_def = '\'' >> base[quoted_fn];
    BOOST_SPIRIT_DEFINE(quoted);
    BOOST_SPIRIT_DEFINE(val);

    auto const program_def = *val;
    BOOST_SPIRIT_DEFINE(program);

    auto const space_skip = +( lit(" ") | '\t' | '\r' | '\n');
    // a define
    auto const define_def = '(' >> lexeme[lit("def") > space_skip > symbol > space_skip ] > val > ')' ;
    BOOST_SPIRIT_DEFINE(define);

    // A vector
    auto const vector_def = '[' > program_def > ']';
    BOOST_SPIRIT_DEFINE(vector);

    // A pair
    struct pair_class : x3::annotate_on_success { };
    rule<pair_class, ast::pair> const pair("pair");
    auto const pair_def = val >> val;
    BOOST_SPIRIT_DEFINE(pair);

    // A binding pair
    rule<pair_class, ast::pair> const binding("binding");
    auto const binding_def = as<ast::pair>[as<ast::val>[symbol] >> val];

    // Bindings
    struct bindings_class : x3::annotate_on_success { };
    rule<bindings_class, ast::bindings> const bindings("bindings");
    auto const bindings_def = '[' > *binding > ']';
    BOOST_SPIRIT_DEFINE(bindings);

    // A let
    auto constexpr let_bindings = [](auto& _ctx) { 

        auto it = _attr(_ctx).mBindings.iterator();

        while( auto p = it->next() ) {

        }

        ctx_info(_ctx); 
    };
    auto const let_def = ('(' >> lit("let") > bindings > program > ')')[let_bindings];
    BOOST_SPIRIT_DEFINE(let);

    // Args
    auto const args_def = '[' >> *as<ast::val>[symbol] > -('&' > symbol) > ']';
    BOOST_SPIRIT_DEFINE(args);

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
    auto const macro_def = '(' >> lit("defmacro") > symbol > args > base > ')';
    BOOST_SPIRIT_DEFINE(macro);

    /* auto constexpr to_typed_arg_string = [](auto & _ctx) { */
    /*     using namespace std; */
    /*     std::string ret; */

    /*     auto & a =_attr(_ctx); */
    /*     auto sym  = at_c<0>(a); */
    /*     auto type = at_c<1>(a); */

    /*     ret += sym.mName + ":"; */

    /*     if (type) { */
    /*         ret += type->mName; */
    /*     } else { */
    /*         ret += "no type"; */
    /*         _pass(_ctx) = false; */
    /*     } */

    /*     _val(_ctx) = ret; */
    /* }; */

    /* struct typed_arg_class { }; */
    /* rule<typed_arg_class, std::string> const typed_arg = "typed_arg"; */
    /* auto const typed_arg_def = lexeme[symbol >> -(':' >> symbol)][to_typed_arg_string]; */
    /* BOOST_SPIRIT_DEFINE(typed_arg); */

    /* struct typed_args_class { }; */
    /* rule<typed_args_class, std::string> const typed_args = "typed_args"; */
    /* auto const typed_args_def = as<std::string>['[' >> *typed_arg >> ']']; */
    /* BOOST_SPIRIT_DEFINE(typed_args); */

    /* auto const tester_def = '|' >> typed_args >> '|'; */
    /* BOOST_SPIRIT_DEFINE(tester); */
}

#endif /* end of include guard: GRAMMAR_H_SLEB5MGA */
