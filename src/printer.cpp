#include "printer.h"
#include <stdio.h>

#include <iostream>

namespace ast {
    using std::endl;

    printer::printer( std::ostream &_out ) : printer_base( _out ) {
    }

    void printer::operator()( ast::nil ) const {
        BOOST_ASSERT( 0 );
    }

    void printer::operator()( unsigned int _v ) {
        mOut << _v;
    }

    void printer::operator()( double _v ) {
        mOut << _v;
    }

    void printer::operator()( std::string _v ) {
        mOut << _v;
    }

    void printer::operator()( ast::symbol const &_v ) {
        mOut << _v.get();
    }

    void printer::operator()( ast::list const &_list ) {
        mOut << "(";
        ( *this )( _list.mForms );
        mOut << ")";
    }

    void printer::operator()( ast::form const &_v ) {
        boost::apply_visitor( *this, _v );
    }

    void printer::operator()( ast::form_list const &_v ) {

        for ( auto const &f : _v ) {
            ( *this )( f );
            mOut << " ";
        }
    }

    void printer::operator()( ast::program const &_p ) {
        ( *this )( _p.mForms );
        mOut << endl;
    }
}
