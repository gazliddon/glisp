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


}

#endif /* end of include guard: GRAMMAR_ATOMS_H_O7UNKZGL */
