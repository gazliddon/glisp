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

    struct symbol_class;
    struct program_class;
    struct list_class;
    struct form_class;

    rule<symbol_class, ast::symbol> const symbol( "symbol" );

    rule<list_class, ast::list> const list( "list" );

    rule<program_class, ast::program> const program( "program" );

    rule<form_class, ast::form> const form( "form" );

    // Form
    auto const form_def = symbol | uint_ | double_ | hex | list;

    // Symbol
    auto const start_char = (alpha | char_("+-*/_!?"));
    auto const rest_char = (start_char | alnum );
    auto const symbol_def = lexeme[start_char >> *rest_char];

    // A list
    auto const list_def = '(' >> *form >> ')';

    // a program, just a load of forms
    auto const program_def = *form;

    BOOST_SPIRIT_DEFINE( program, form, symbol, list);
}


#endif /* end of include guard: GRAMMAR_H_SLEB5MGA */
