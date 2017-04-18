#ifndef AST_H_4GSXUIIF
#define AST_H_4GSXUIIF

#include <list>
#include <map>
#include <set>
#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast {
  namespace x3 = boost::spirit::x3;
  using x3::forward_ast;

  struct nil {};

  struct list;
  struct vector;
  struct map;
  struct meta;
  struct set;
  struct fn;

  struct symbol {
    std::string mName;
  };

  struct boolean {
    std::string mVal;
  };

  struct keyword {
    symbol mSym;
  };

  struct hint {
    symbol mSym;
  };

  struct special {
    std::string mName;
  };

  struct atom
      : x3::variant<special, boolean, symbol, keyword, std::string,
                    forward_ast<fn>, hint, nil, unsigned int, double, char> {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct form
      : x3::variant<atom, forward_ast<set>, forward_ast<list>,
                    forward_ast<vector>, forward_ast<map>, forward_ast<meta>> {
    using base_type::base_type;
    using base_type::operator=;
  };

  typedef std::list<form> form_list;

  struct map_entry {
    form mKey;
    form mValue;
  };

  typedef std::list<map_entry> map_list;

  struct list {
    form_list mForms;
  };

  struct vector {
    form_list mForms;
  };

  struct map {
    map_list mHashMap;
  };

  struct meta {
    map_list mHashMap;
  };

  struct set {
    form_list mForms;
  };

  struct fn {
    size_t mNumberOfArgs;
    std::function<void( size_t _nargs, form_list const &_args )> mFn;
  };

  struct program {
    form_list mForms;
  };

  // print function for debugging
  inline std::ostream &operator<<( std::ostream &out, nil ) {
    out << "nil";
    return out;
  }
}

#endif /* end of include guard: AST_H_4GSXUIIF */
