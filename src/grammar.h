#ifndef GRAMMAR_H_SLEB5MGA
#define GRAMMAR_H_SLEB5MGA

#include "ast_adapted.h"

#pragma GCC diagnostic ignored "-Wparentheses"

namespace grammar {
    namespace x3 = boost::spirit::x3;

    using x3::uint_;
    using x3::int_;
    using x3::double_;
    using x3::char_;
    using x3::hex;
    using x3::lexeme;
    using x3::rule;
    using x3::alpha;
    using x3::alnum;

    struct atom_class;
    struct symbol_class;
    struct keyword_class;
    struct program_class;
    struct list_class;
    struct form_class;
    struct vector_class;
    struct map_class;
    struct map_entry_class;
    struct set_class;

    rule<symbol_class, ast::symbol> const symbol( "symbol" );

    rule<keyword_class, ast::keyword> const keyword( "keyword" );
    rule<atom_class, ast::atom> const atom( "atom" );

    rule<form_class, ast::form> const form( "form" );

    rule<list_class, ast::list> const list( "list" );
    rule<vector_class, ast::vector> const vector( "vector" );

    rule<map_class, ast::map> const map( "map" );
    rule<map_entry_class, ast::map_entry> const map_entry( "map_entry" );

    rule<set_class, ast::set> const set( "set" );

    rule<program_class, ast::program> const program( "program" );

    // Symbol
    auto const start_char = ( alpha | char_( "+-*/_!?" ) );
    auto const rest_char  = ( start_char | alnum );
    auto const symbol_def = lexeme[ start_char >> *rest_char ];

    // keyword
    auto const keyword_def = lexeme[ ':' > symbol ];

    // atom
    auto const atom_def = symbol | keyword | uint_ | double_ | hex;

    // Form
    auto const form_def = atom | list | vector | map | set;

    // A list
    auto const list_def = '(' >> *form >> ')';

    // A vector
    auto const vector_def = '[' >> *form >> ']';

    // A set
    auto const set_def = "#{" >> *form >> '}';

    // a map and map entry
    auto const map_entry_def = form >> form;
    auto const map_def       = '{' >> *map_entry >> '}';

    // a program, just a load of forms
    auto const program_def = *form;

    BOOST_SPIRIT_DEFINE( program,
                         form,
                         symbol,
                         keyword,
                         atom,
                         list,
                         vector,
                         map_entry,
                         map,
                         set );
}

#endif /* end of include guard: GRAMMAR_H_SLEB5MGA */
