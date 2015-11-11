///////////////////////////////////////////////////////////////////////////////
//#define BOOST_SPIRIT_X3_DEBUG

#include <iostream>
#include <string>
#include <list>
#include <numeric>

#include "grammar.h"
#include "printer.h"

int main() {
    namespace x3 = boost::spirit::x3;
    namespace ascii = x3::ascii;

    using ascii::space_type;
    using ast::program;
    using ast::printer;
    using std::string;

    std::cout << "Glisp lisp parser\n";
    std::cout << "Type an expression...or [q or Q] to quit\n\n";

    string str;
    space_type space;
    printer printIt; 

    while ( std::getline( std::cin, str ) ) {

        if ( str.empty() || str[ 0 ] == 'q' || str[ 0 ] == 'Q' )
            break;

        program outputAst; 

        auto iter = str.begin(), end = str.end();

        bool r = phrase_parse( iter, end, grammar::program, space, outputAst );

        if ( r && iter == end ) {

            // Woo - worked
            // print out the AST
            std::cout << "Parsing succeeded\n";
            printIt( outputAst );

            /* mach.execute( code ); */
            /* std::cout << "\nResult: " << mach.top() << std::endl; */
        } else {
            std::string rest( iter, end );
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }
    return 0;
}
