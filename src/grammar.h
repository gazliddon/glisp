#ifndef GRAMMAR_H_SLEB5MGA
#define GRAMMAR_H_SLEB5MGA

#include "ast_adapted.h"
#include "grammar_atoms.h"

#pragma GCC diagnostic ignored "-Wparentheses"

namespace grammar {

    namespace x3 = boost::spirit::x3;

    using x3::alnum;
    using x3::alpha;
    using x3::space;
    using x3::lexeme;
    using x3::rule;
    /* using x3::space; */
    /* using x3::string; */

    // composite types
    struct form_class;
    struct application_class;
    struct vector_class;
    struct map_entry_class;
    struct map_class;
    struct meta_class;
    struct set_class;
    struct program_class;
    struct val_class;
    struct define_class;

    // bool
    struct boolean_class;
    rule<boolean_class, ast::boolean> const boolean("boolean");
    auto const boolean_def = string("true") | string("false");
    BOOST_SPIRIT_DEFINE(boolean);



    // Special forms
    // need special evaluation
    struct special_class;
    rule<special_class, ast::special> const special("special");
    auto const special_def = string("def") | string("let") | string("fn")
        | string("quote") | string("if") | string("and")
        | string("or");
    BOOST_SPIRIT_DEFINE(special);

    // Strings
    struct str_class;
    rule<str_class, std::string> const str("str");
    auto const str_def = lexeme['"' >> *(char_ - '"') >> '"'];
    BOOST_SPIRIT_DEFINE(str);

    // Symbol
    //

    struct symbol_class;
    rule<symbol_class, ast::symbol> const symbol("symbol");
    auto const symbol_def = lexeme[alpha >> *alnum];
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
    rule<define_class, ast::define> const define("define");
    rule<application_class, ast::application> const application("application");

    // clang-format off

    // A Val
    rule<val_class, ast::val> const val("val");
    auto const val_def =
          special
        | boolean
        | symbol
        | str
        | double_
        | character
        | application
        ;
    BOOST_SPIRIT_DEFINE(val);

    // clang-format on
    

    // Define!
    auto const define_def = '(' >> string("define") >> val >> ')';
    BOOST_SPIRIT_DEFINE(define);

    auto const val_list = val % space;

    // List
    auto const application_def = '(' >> (application | symbol) >> *val >> ')';
    BOOST_SPIRIT_DEFINE(application);

    // A vector
    auto const vector_def = '[' >> lexeme[val % " "] >> ']';
    BOOST_SPIRIT_DEFINE(vector);

    // A set
    auto const set_def = "#{" >> lexeme[val % " "] >> '}';
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
