#include "printer.h"
#include <stdio.h>

#include <iostream>

namespace ast {
    using boost::apply_visitor;

    printer::printer( std::ostream &_out ) : printer_base( _out ) {
    }

    void printer::operator()( ast::keyword const &_keyword ) const {
        mOut << ":" << _keyword.mSym.get();
    }

    void printer::operator()( bool _v ) const {
        if ( _v ) {
            mOut << "true";
        } else {
            mOut << "false";
        }
    }

    void printer::operator()( char _v ) const {
        mOut << _v;
    }

    void printer::operator()( ast::nil const & ) const {
        BOOST_ASSERT( 0 );
    }

    void printer::operator()( unsigned int _v ) const {
        mOut << _v;
    }

    void printer::operator()( double _v ) const {
        mOut << _v;
    }

    void printer::operator()( std::string const &_v ) const {
        mOut << _v;
    }

    void printer::operator()( ast::symbol const &_v ) const {
        mOut << _v.get();
    }

    void printer::operator()( ast::list const &_list ) const {
        renderCollection( _list.mForms, "(", ")" );
    }

    void printer::operator()( ast::vector const &_vector ) const {
        renderCollection( _vector.mForms, "[", "]" );
    }

    void printer::operator()( ast::map const &_map ) const {
        renderCollection( _map.mHashMap, "{", "}" );
    }


    void printer::operator() ( ast::set const & _set) const {
        renderCollection( _set.mForms, "#{", "}" );
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
