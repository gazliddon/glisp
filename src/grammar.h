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
    using x3::lit;
    using x3::rule;

    struct symbol_class;
    struct program_class;
    struct list_class;
    struct form_class;
    struct special_form_class;

    rule<symbol_class, ast::symbol> const symbol( "symbol" );
    rule<list_class, ast::program> const list( "list" );
    rule<program_class, ast::program> const program( "program" );
    rule<form_class, ast::form> const form( "form" );
    rule<special_form_class, ast::special_form> const
        special_form( "special_form" );

    // Form
    auto const form_def = symbol | uint_ | double_ | hex | special_form | list;

    // Symbol
    auto const symbol_def =
        lexeme[ char_( "a-zA-Z!*+_" ) >> *( char_( '-' ) | char_( "0-9" ) ) ];

    // List
    auto const list_def = '(' >> *form >> ')';

    // special forms
    auto const special_form_def = '(' >> ( x3::string( "define" ) |
                                           x3::string( "lambda" ) ) >>
                                  *form >> ')';

    // a program
    auto const program_def = *form;

    BOOST_SPIRIT_DEFINE( program, form, symbol, list, special_form );
}


#endif /* end of include guard: GRAMMAR_H_SLEB5MGA */
