#include "printer.h"
#include <stdio.h>

#include <iostream>

namespace ast {

    using std::cout;

    std::string printer::indent() const {
        return std::string( depth, ' ' );
    }

    printer::printer() {
        depth = 0;
    }

    void printer::operator()( ast::nil ) const {
        BOOST_ASSERT( 0 );
    }

    void printer::operator()( unsigned int _v ) {
        using namespace std;
        cout << indent() << "int: " << _v << endl;
    }

    void printer::operator()( double _v ) {
        using namespace std;
        cout << indent() << "double: " << _v << endl;
    }

    void printer::operator()( std::string _v ) {
        using namespace std;
        cout << indent() << "string: " << _v << endl;
    }

    void printer::operator()( ast::symbol const &_v ) {
        using namespace std;
        cout << indent() << "symbol: " << _v.get() << endl;
    }

    void printer::operator()( ast::form const &_v ) {
        boost::apply_visitor( *this, _v );
    }

    void printer::operator()( ast::form_list const &_v ) {
        using namespace std;
        cout << indent() << "list begin: " << endl;
        depth++;
        for ( auto const &f : _v ) {
            ( *this )( f );
        }
        depth--;
        cout << indent() << "list end: " << endl;
    }

    void printer::operator()( ast::special_form const &_v ) {
        using namespace std;
        cout << indent() << "special form begin: " << _v.mSpeshName << endl;
        depth++;

        for ( auto const &_e : _v.mForms ) {
            ( *this )( _e );
        }
        depth--;
        cout << indent() << "special form end: " << _v.mSpeshName << endl;
    }

    void printer::operator()( ast::program const &_p ) {
        using namespace std;

        cout << indent() << "program start" << endl;
        depth++;
        for ( auto const &f : _p.mForms ) {
            ( *this )( f );
        }
        depth--;
        cout << indent() << "program end" << endl;
    }

    /* void operator()( ast::operation const &x ) const { */
    /*     boost::apply_visitor( *this, x.operand_ ); */
    /*     switch ( x.operator_ ) { */
    /*         case '+': */
    /*             code.push_back( op_add ); */
    /*             break; */
    /*         case '-': */
    /*             code.push_back( op_sub ); */
    /*             break; */
    /*         case '*': */
    /*             code.push_back( op_mul ); */
    /*             break; */
    /*         case '/': */
    /*             code.push_back( op_div ); */
    /*             break; */
    /*         default: */
    /*             BOOST_ASSERT( 0 ); */
    /*             break; */
    /*     } */
    /* } */

    /* void operator()( ast::signed_ const &x ) const { */
    /*     boost::apply_visitor( *this, x.operand_ ); */
    /*     switch ( x.sign ) { */
    /*         case '-': */
    /*             code.push_back( op_neg ); */
    /*             break; */
    /*         case '+': */
    /*             break; */
    /*         default: */
    /*             BOOST_ASSERT( 0 ); */
    /*             break; */
    /*     } */
    /* } */
    /* void operator()( ast::hexnum const &x ) const { */
    /*     uint64_t val = 0; */

    /*     for ( auto &c : x.mText ) { */
    /*         int base = -1; */
    /*         if ( c >= '0' && c <= '9' ) { */
    /*             base = '0'; */
    /*         } else if ( c >= 'a' && c <= 'f' ) { */
    /*             base = 'a' - 10; */
    /*         } else if ( c >= 'A' && c <= 'F' ) { */
    /*             base = 'A' - 10; */
    /*         } */
    /*         val = ( val << 4 ) + ( c - base ); */
    /*     } */

    /*     code.push_back( op_int ); */
    /*     code.push_back( val ); */
    /* }; */

    /* void operator()( ast::symbol const &x ) const { */
    /*     using namespace std; */
    /*     cout << "got a symbol named " << x.mFirst << x.mRest << endl; */
    /* } */

    /* void operator()( ast::expression const &x ) const { */
    /*     boost::apply_visitor( *this, x.first ); */
    /*     for ( ast::operation const &oper : x.rest ) { */
    /*         ( *this )( oper ); */
    /*     } */
    /* } */
}
