#include "printer.h"
#include <stdio.h>

#include <iostream>

namespace ast {
  using boost::apply_visitor;

  printer::printer( std::ostream &_out ) : printer_base( _out ) {
  }

  void printer::operator()( ast::keyword const &_keyword ) const {
    mOut << ":" << _keyword.mSym.mName << ":keyword";
  }

  void printer::operator()( ast::hint const &_keyword ) const {
    mOut << "^" << _keyword.mSym.mName << ":typehint";
  }

  void printer::operator()( ast::boolean const &_val ) const {
    mOut << _val.mVal << ":bool";
  }

  void printer::operator()( char _v ) const {
    mOut << _v << ":char";
  }

  void printer::operator()( ast::nil const & ) const {
    BOOST_ASSERT( 0 );
  }

  void printer::operator()( unsigned int _v ) const {
    mOut << _v << ":int";
  }

  void printer::operator()( double _v ) const {
    mOut << _v << ":double";
  }

  void printer::operator()( std::string const &_v ) const {
    mOut << _v << ":string";
  }

  void printer::operator()( ast::symbol const &_v ) const {
    mOut << _v.mName << ":symbol";
  }

  void printer::operator()( ast::special const &_v ) const {
    mOut << _v.mName << ":special-form";
  }

  void printer::operator()( ast::list const &_list ) const {
    renderCollection( _list.mForms, "(", ")", "list" );
  }

  void printer::operator()( ast::vector const &_vector ) const {
    renderCollection( _vector.mForms, "[", "]", "vector" );
  }

  void printer::operator()( ast::map const &_map ) const {
    renderCollection( _map.mHashMap, "{", "}", "map" );
  }

  void printer::operator()( ast::meta const &_val ) const {
    renderCollection( _val.mHashMap, "^{", "}", "meta" );
  }

  void printer::operator()( ast::set const &_set ) const {
    renderCollection( _set.mForms, "#{", "}", "set" );
  }

  void printer::operator()( ast::map_entry const &_map_entry ) const {
    apply_visitor( *this, _map_entry.mKey );
    mOut << " ";
    apply_visitor( *this, _map_entry.mValue );
  }

  void printer::operator()( ast::form const &_v ) const {
    apply_visitor( *this, _v );
  }

  void printer::operator()( ast::form_list const &_v ) const {
    renderList( _v );
  }

  void printer::operator()( ast::program const &_p ) const {
    ( *this )( _p.mForms );
    mOut << std::endl;
  }

  void printer::operator()( ast::atom const &_atom ) const {
    boost::apply_visitor( *this, _atom );
  }
}
