#ifndef AST_ADAPTED_H_KW9BM5GS
#define AST_ADAPTED_H_KW9BM5GS

#include "ast.h"

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT( ast::symbol, mFirst, mRest )
BOOST_FUSION_ADAPT_STRUCT( ast::program, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::list, mForms )

#endif /* end of include guard: AST_ADAPTED_H_KW9BM5GS */
