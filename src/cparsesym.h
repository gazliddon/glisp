#ifndef CPARSESYM_H_IYTKDQNA
#define CPARSESYM_H_IYTKDQNA

#include "spirit/include/boost/spirit/home/x3.hpp"

#include "ast.h"


namespace glisp {

    namespace x3 = boost::spirit::x3;

    namespace parser {
        class symbol;
        using x3::alpha;
        using x3::alnum;
        using x3::lexeme;
        using x3::raw;

        x3::rule<class symbol, ast::symbol> const symbol = "symbol";

        auto const sym_rest_chars =
            ( alnum | '_' | '-' | '*' | '?' | '!' | '>' | '<' );
        auto const symbol_def = raw[ lexeme[ +alpha ] ];

        BOOST_SPIRIT_DEFINE( symbol );
    }
}

#endif /* end of include guard: CPARSESYM_H_IYTKDQNA */
