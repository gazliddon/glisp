#ifndef AST_ADAPTED_H_KW9BM5GS
#define AST_ADAPTED_H_KW9BM5GS

#include "ast.h"

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT( ast::symbol, mId)
BOOST_FUSION_ADAPT_STRUCT( ast::keyword, mSym )
BOOST_FUSION_ADAPT_STRUCT( ast::hint, mSym )
BOOST_FUSION_ADAPT_STRUCT( ast::program, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::vector, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::map_entry, mKey, mValue )
BOOST_FUSION_ADAPT_STRUCT( ast::map, mHashMap )
BOOST_FUSION_ADAPT_STRUCT( ast::set, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::meta, mHashMap )
BOOST_FUSION_ADAPT_STRUCT( ast::define, mSym, mVal )
BOOST_FUSION_ADAPT_STRUCT( ast::lambda, mDocString, mArgs, mBody )
BOOST_FUSION_ADAPT_STRUCT( ast::sexp, mForms )
BOOST_FUSION_ADAPT_STRUCT( ast::nil )
BOOST_FUSION_ADAPT_STRUCT( ast::macro, mSym, mArgs, mVal )
BOOST_FUSION_ADAPT_STRUCT( ast::args, mArgs, mExtra)
BOOST_FUSION_ADAPT_STRUCT( ast::let, mBindings, mBody)
BOOST_FUSION_ADAPT_STRUCT( ast::pair, mFirst, mSecond )
BOOST_FUSION_ADAPT_STRUCT( ast::bindings, mBindings)

#endif /* end of include guard: AST_ADAPTED_H_KW9BM5GS */
