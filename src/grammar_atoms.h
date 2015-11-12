#ifndef GRAMMAR_ATOMS_H_O7UNKZGL
#define GRAMMAR_ATOMS_H_O7UNKZGL

#include "ast_adapted.h"

#pragma GCC diagnostic ignored "-Wparentheses"

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

    // Symbol
    struct symbol_class;
    rule<symbol_class, ast::symbol> const symbol( "symbol" );
    auto const start_char = ( alpha | char_( "+-*/_!?" ) );
    auto const rest_char  = ( start_char | alnum );
    auto const symbol_def = lexeme[ &start_char >> +rest_char ];
    BOOST_SPIRIT_DEFINE( symbol );

    // Character
    struct character_class;
    rule<character_class, char> const character( "character" );
    auto const character_def = lit( '\\' ) >> char_;
    BOOST_SPIRIT_DEFINE( character );

    // keyord
    struct keyword_class;
    rule<keyword_class, ast::keyword> const keyword( "keyword" );
    auto const keyword_def = lexeme[ ':' > symbol ];
    BOOST_SPIRIT_DEFINE( keyword );

    // atom
    struct atom_class;
    rule<atom_class, ast::atom> const atom( "atom" );
    auto const atom_def = symbol | keyword | uint_ | double_ | hex | character | bool_;
    BOOST_SPIRIT_DEFINE( atom );
}

#endif /* end of include guard: GRAMMAR_ATOMS_H_O7UNKZGL */
