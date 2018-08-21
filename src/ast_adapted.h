#ifndef AST_ADAPTED_H_KW9BM5GS
#define AST_ADAPTED_H_KW9BM5GS

#include "ast.h"

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT( ast::symbol, mStart, mName )
BOOST_FUSION_ADAPT_STRUCT( ast::keyword, mSym )
BOOST_FUSION_ADAPT_STRUCT( ast::hint, mSym )
BOOST_FUSION_ADAPT_STRUCT( ast::program, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::application, mFunc, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::vector, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::map_entry, mKey, mValue )
BOOST_FUSION_ADAPT_STRUCT( ast::map, mHashMap )
BOOST_FUSION_ADAPT_STRUCT( ast::set, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::boolean, mVal )
BOOST_FUSION_ADAPT_STRUCT( ast::meta, mHashMap )
BOOST_FUSION_ADAPT_STRUCT( ast::sp_define, mSym, mVal )
BOOST_FUSION_ADAPT_STRUCT( ast::sp_if, mPred,mTrue, mFalse )
BOOST_FUSION_ADAPT_STRUCT( ast::sp_or, mVals)
BOOST_FUSION_ADAPT_STRUCT( ast::sp_and, mVals)
BOOST_FUSION_ADAPT_STRUCT( ast::sp_lambda, mArgs, mForms)
BOOST_FUSION_ADAPT_STRUCT( ast::sp_quote, mVal)
BOOST_FUSION_ADAPT_STRUCT( ast::sp_list, mVals)

#endif /* end of include guard: AST_ADAPTED_H_KW9BM5GS */
