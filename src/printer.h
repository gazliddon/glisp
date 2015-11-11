#ifndef PRINTER_H_9ABCZ08P
#define PRINTER_H_9ABCZ08P

#include "ast.h"
#include <string>

// Ast printer
namespace ast {
    struct printer {

        typedef void result_type;

        printer();

        void operator()( ast::nil ) const;

        void operator()( unsigned int _v );

        void operator()( double _v );

        void operator()( std::string _v );

        void operator()( ast::symbol const &_v );

        void operator()( ast::form const &_v );

        void operator()( ast::form_list const &_v );

        void operator()( ast::special_form const &_v );

        void operator()( ast::program const &_p );

        unsigned int depth;

        std::string indent() const;
    };
}
#endif /* end of include guard: PRINTER_H_9ABCZ08P */
