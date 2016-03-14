#ifndef GRAMMAR_H_SLEB5MGA
#define GRAMMAR_H_SLEB5MGA

#include "ast_adapted.h"
#include "grammar_atoms.h"

#pragma GCC diagnostic ignored "-Wparentheses"

namespace grammar {
    namespace x3 = boost::spirit::x3;

    using x3::lexeme;
    using x3::rule;
    using x3::alpha;
    using x3::alnum;
    using x3::space;
    using x3::string;
    using x3::bool_;

    // composite types
    struct form_class;
    struct list_class;
    struct vector_class;
    struct map_entry_class;
    struct map_class;
    struct set_class;
    struct program_class;

    rule<list_class, ast::list> const list( "list" );
    rule<form_class, ast::form> const form( "form" );
    rule<vector_class, ast::vector> const vector( "vector" );
    rule<map_class, ast::map> const map( "map" );
    rule<map_entry_class, ast::map_entry> const map_entry( "map_entry" );
    rule<set_class, ast::set> const set( "set" );
    rule<program_class, ast::program> const program( "program" );


    // Form
    auto const form_def =
        list | vector | map | set | list | atom;
    BOOST_SPIRIT_DEFINE( form );

    // List
    auto const list_def = '(' >> *form >> ')';
    BOOST_SPIRIT_DEFINE( list );

    // A vector
    auto const vector_def = '[' >> *form >> ']';
    BOOST_SPIRIT_DEFINE( vector );

    // A set
    auto const set_def = "#{" >> *form >> '}';
    BOOST_SPIRIT_DEFINE( set );

    // a map entry
    auto const map_entry_def = form >> form;
    BOOST_SPIRIT_DEFINE( map_entry );

    // a map
    auto const map_def = '{' >> *map_entry >> '}';
    BOOST_SPIRIT_DEFINE( map );

    // a program, just a load of forms
    auto const program_def = *form;
    BOOST_SPIRIT_DEFINE( program );
}

#endif /* end of include guard: GRAMMAR_H_SLEB5MGA */
