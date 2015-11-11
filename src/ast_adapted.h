#ifndef AST_ADAPTED_H_KW9BM5GS
#define AST_ADAPTED_H_KW9BM5GS

#include "ast.h"

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT( ast::symbol, mFirst, mRest )
BOOST_FUSION_ADAPT_STRUCT( ast::keyword, mSym )
BOOST_FUSION_ADAPT_STRUCT( ast::program, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::list, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::vector, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::map_entry, mKey, mValue )
BOOST_FUSION_ADAPT_STRUCT( ast::map, mHashMap )
BOOST_FUSION_ADAPT_STRUCT( ast::set, mForms )

#endif /* end of include guard: AST_ADAPTED_H_KW9BM5GS */
