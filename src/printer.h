#ifndef PRINTER_H_9ABCZ08P
#define PRINTER_H_9ABCZ08P

#include "ast.h"
#include <string>
#include <iostream>
#include <assert.h>

// Ast printer
namespace ast {
    struct printer_base {
        typedef void result_type;
        std::ostream &mOut;
        printer_base( std::ostream &_out ) : mOut( _out ) {
        }
    };

    struct printer : printer_base {
        printer( std::ostream &_out = std::cout );

        void operator()( ast::symbol const &_v ) const;

        void operator()( ast::keyword const &_keyword ) const;

        void operator()( std::string const &_v ) const;

        void operator()( unsigned int _v ) const;

        void operator()( double _v ) const;

        void operator()( bool _v ) const;

        void operator()( char _v ) const;

        void operator()( ast::nil const & ) const;

        void operator()( ast::form const &_v ) const;

        void operator()( ast::program const &_p ) const;

        void operator()( ast::list const &_list ) const;

        void operator()( ast::vector const &_vector ) const;

        void operator()( ast::map_entry const &_map_entry ) const;

        void operator()( ast::map const &_map ) const;

        void operator()( ast::form_list const &_v ) const;

        void operator()( ast::atom const &_atom ) const;

        void operator()( ast::set const &_set ) const;

        void operator()( ast::special_form const &_sf ) const;

        void operator()(ast::fn const & _fn) const {
            assert(false);
        }

        void operator()(ast::call const & _call) const {
            assert(false);
        }

        template <typename T>
        void renderList( T const &_col ) const {
            auto it = _col.begin(), end = _col.end();

            while ( it != end ) {
                ( *this )( *it++ );
                if ( it != end ) {
                    mOut << " ";
                }
            }
        }

        template <typename T>
        void renderCollection( T const &_col,
                               std::string const &_pre,
                               std::string const &_post,
                               std::string const & _type
                               ) const {
            mOut << _pre;
            renderList( _col );
            mOut << _post << ":" << _type;
        }
    };

    struct ast_dumper {

        ast_dumper( std::ostream &_out = std::cout );

        void operator()( ast::nil ) const;
        void operator()( unsigned int _v );
        void operator()( double _v );
        void operator()( std::string _v );
        void operator()( ast::symbol const &_v );
        void operator()( ast::form const &_v );
        void operator()( ast::program const &_p );
        void operator()( ast::list const &_list );
        void operator()( ast::form_list const &_v );

        template <typename T>
        void operator()( T const &_v ) {
            assert( !"NOT HANDLED" );
        }
    };
}

#endif /* end of include guard: PRINTER_H_9ABCZ08P */
