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
  using x3::string;

  // bool
  struct boolean_class;
  rule<boolean_class, ast::boolean> const boolean( "boolean" );
  auto const boolean_def = string( "true" ) | string( "false" );
  BOOST_SPIRIT_DEFINE( boolean );

  // Special forms
  // need special evaluation

  struct special_class;
  rule<special_class, ast::special> const special( "special" );

  auto const special_def = string( "def" ) | string( "let" ) | string( "fn" ) |
                           string( "quote" ) | string( "cond" ) |
                           string( "if" ) | string( "and" ) | string( "or" );

  BOOST_SPIRIT_DEFINE( special );

  // Strings
  struct str_class;
  rule<str_class, std::string> const str( "str" );
  auto const str_def = lexeme[ '"' >> *( char_ - '"' ) >> '"' ];
  BOOST_SPIRIT_DEFINE( str );

  // Symbol
  struct symbol_class;
  rule<symbol_class, ast::symbol> const symbol( "symbol" );
  auto const extra_chars = char_( "_><?!+-*" );
  auto const start_char = ( alpha | extra_chars );
  auto const rest_char = ( alnum | extra_chars );
  auto const all_sym = start_char >> *( rest_char );
  auto const symbol_def = lexeme[ ( start_char >> *rest_char )];

  BOOST_SPIRIT_DEFINE( symbol );

  // Character
  struct character_class;
  rule<character_class, char> const character( "character" );
  auto const character_def = lexeme[lit( '\\' ) >> char_];
  BOOST_SPIRIT_DEFINE( character );

  // keyord
  struct keyword_class;
  rule<keyword_class, ast::keyword> const keyword( "keyword" );
  auto const keyword_def = lexeme[ ':' > symbol ];
  BOOST_SPIRIT_DEFINE( keyword );

  // Type hint
  struct hint_class;
  rule<str_class, ast::hint> const hint( "hint" );
  auto const hint_def = lexeme[ '^' > symbol ];
  BOOST_SPIRIT_DEFINE( hint );

  // atom
  struct atom_class;
  rule<atom_class, ast::atom> const atom( "atom" );
  auto const atom_def = uint_ | double_ | hint | str | special | boolean |
                        keyword | symbol | hex | character;
  BOOST_SPIRIT_DEFINE( atom );
}

#endif /* end of include guard: GRAMMAR_ATOMS_H_O7UNKZGL */
