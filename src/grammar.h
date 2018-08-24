#ifndef GRAMMAR_H_SLEB5MGA
#define GRAMMAR_H_SLEB5MGA

#include "ast_adapted.h"
#include "grammar_atoms.h"

#pragma GCC diagnostic ignored "-Wparentheses"

namespace grammar {

    namespace x3 = boost::spirit::x3;

    using x3::alnum;
    using x3::alpha;
    using x3::lexeme;
    using x3::rule;
    using x3::space;
    /* using x3::space; */
    /* using x3::string; */

    // composite types
    struct form_class;
    struct application_class;
    struct vector_class;
    struct map_entry_class;
    struct binding_class;
    struct map_class;
    struct meta_class;
    struct set_class;
    struct program_class;
    struct val_class;
    struct sp_define_class;
    struct sp_or_class;
    struct sp_if_class;
    struct sp_and_class;
    struct sp_lambda_class;
    struct sp_quote_class;
    struct sp_list_class;
    struct sp_let_class;
    // bool
    //
    struct boolean_class;
    rule<boolean_class, ast::boolean> const boolean("boolean");
    auto const boolean_def = string("true") | string("false");
    BOOST_SPIRIT_DEFINE(boolean);

    // Strings
    struct str_class;
    rule<str_class, std::string> const str("str");
    auto const str_def = lexeme['"' >> *(char_ - '"') >> '"'];
    BOOST_SPIRIT_DEFINE(str);

    // Symbol
    struct symbol_class;
    rule<symbol_class, ast::symbol> const symbol("symbol");
    auto const echars     = char_("=_.?!*+-/><");
    auto const symbol_def = lexeme[(alpha | echars) >> *(alnum | echars)];
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
    rule<sp_or_class, ast::sp_or> const sp_or("sp_or");
    rule<sp_define_class, ast::sp_define> const sp_define("sp_define");
    rule<sp_and_class, ast::sp_and> const sp_and("sp_and");
    rule<sp_if_class, ast::sp_if> const sp_if("sp_if");
    rule<sp_lambda_class, ast::sp_lambda> const sp_lambda("sp_lambda");
    rule<sp_quote_class, ast::sp_quote> const sp_quote("sp_quote");
    rule<sp_list_class, ast::sp_list> const sp_list("sp_list");
    rule<application_class, ast::application> const application("application");
    rule<program_class, ast::program> const program("program");
    rule<sp_let_class, ast::sp_let> const sp_let("sp_let");

    // clang-format off

    // A Val
    rule<val_class, ast::val> const val("val");
    auto const val_def =
        boolean
        | sp_let
        | sp_lambda
        | sp_or
        | sp_and
        | sp_define
        | sp_if
        | sp_list
        | application
        | map
        | vector
        | keyword
        | symbol
        | str
        | double_
        | character
        | sp_quote
        ;

    BOOST_SPIRIT_DEFINE(val);

    // clang-format on
    auto const program_def = *val;
    BOOST_SPIRIT_DEFINE(program);

    auto const bo = lit('(');
    auto const qu = lit('\'');
    auto const bc = lit(')');

    // Special forms
    // need special evaluation

    
    // a binding
    rule<binding_class, ast::binding> const binding("binding");
    auto const binding_def = symbol > val;
    BOOST_SPIRIT_DEFINE(binding);

    // Let
    auto const sp_let_def = bo >> lit("let") >> '[' >> *binding >> ']' >> val >> bc;
    BOOST_SPIRIT_DEFINE(sp_let);

    // list - horrid quote bodge
    auto const sp_list_def = bo >> lit("list") > *(val) > bc;
    BOOST_SPIRIT_DEFINE(sp_list);

    // Quote!
    // TODO fix this, parses a list as an application

    auto const sp_quote_def = qu > val;
    BOOST_SPIRIT_DEFINE(sp_quote);

    // Define!
    auto const sp_lambda_def
        = bo >> (lit("lambda") | lit("fn")) >> vector >> *val >> bc;
    BOOST_SPIRIT_DEFINE(sp_lambda);

    // Define!
    auto const sp_if_def = bo >> lit("if") > val > val > val > bc;
    BOOST_SPIRIT_DEFINE(sp_if);

    // Or
    auto const sp_or_def = '(' >> lit("or") > +val > ')';
    BOOST_SPIRIT_DEFINE(sp_or);

    // and
    auto const sp_and_def = '(' >> lit("and") > +val > ')';
    BOOST_SPIRIT_DEFINE(sp_and);

    // Define!
    auto const sp_define_def
        = '(' >> (lit("def") | lit("define")) > symbol > val > ')';
    BOOST_SPIRIT_DEFINE(sp_define);

    // List
    auto const application_def = '(' >> (application | symbol) >> *val > ')';
    BOOST_SPIRIT_DEFINE(application);

    // A vector
    auto const vector_def = '[' > *val > ']';
    BOOST_SPIRIT_DEFINE(vector);

    // A set
    auto const set_def = "#{" >> lexeme[val % " "] > '}';
    BOOST_SPIRIT_DEFINE(set);

    // a map entry
    rule<map_entry_class, ast::map_entry> const map_entry("map_entry");
    auto const map_entry_def = val > val;
    BOOST_SPIRIT_DEFINE(map_entry);

    // a map
    auto const map_def = '{' >> *map_entry >> '}';
    BOOST_SPIRIT_DEFINE(map);

    // meta data
    auto const meta_def = lit("^{") >> *map_entry >> '}';
    BOOST_SPIRIT_DEFINE(meta);

}

#endif /* end of include guard: GRAMMAR_H_SLEB5MGA */
