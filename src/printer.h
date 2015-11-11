#ifndef PRINTER_H_9ABCZ08P
#define PRINTER_H_9ABCZ08P

#include "ast.h"
#include <string>
#include <iostream>

// Ast printer
namespace ast {
    struct printer_base {
        typedef void result_type;
        std::ostream & mOut;
        printer_base(std::ostream & _out) :
            mOut(_out) {}

    };

    struct printer : printer_base {
        printer( std::ostream & _out = std::cout );

        void operator()( ast::nil ) const;

        void operator()( unsigned int _v );

        void operator()( double _v );

        void operator()( std::string _v );

        void operator()( ast::symbol const &_v );

        void operator()( ast::form const &_v );

        void operator()( ast::program const &_p );

        void operator()(ast::list const & _list);

        void operator()( ast::form_list const &_v );
    };

    struct ast_dumper {

        ast_dumper( std::ostream & _out = std::cout );

        void operator()( ast::nil ) const;
        void operator()( unsigned int _v );
        void operator()( double _v );
        void operator()( std::string _v );
        void operator()( ast::symbol const &_v );
        void operator()( ast::form const &_v );
        void operator()( ast::program const &_p );
        void operator()(ast::list const & _list);
        void operator()( ast::form_list const &_v );
    };
}
#endif /* end of include guard: PRINTER_H_9ABCZ08P */
