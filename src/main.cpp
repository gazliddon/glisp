///////////////////////////////////////////////////////////////////////////////
//#define BOOST_SPIRIT_X3_DEBUG

#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>

#include "grammar.h"
#include "printer.h"

#include "eval.h"
#include "run.h"

// I need to walk my ast
//

namespace glisp {

    using std::cout;
    using std::endl;

    std::string read(std::istream& _in = std::cin) {
        std::string ret;
        std::getline(_in, ret);
        return ret;
    }

    ast::val eval(std::string const& _str) {
        using boost::spirit::x3::ascii::space_type;

        ast::val ast;

        space_type space;

        auto iter = _str.begin(), end = _str.end();
        bool r = phrase_parse(iter, end, grammar::val, space, ast);

        if (r && iter == end) {
        } else {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
            cout << *iter << "\n";
        }

        return ast;
    }

    ast::program eval_ast(ast::program const& _prog) {
        return _prog;
    }

    template <typename T>
    void print(T const& _ast, std::ostream& _out = std::cout) {
        ast::printer printIt(_out);
        boost::apply_visitor(printIt, _ast);
    }

    void repl() {
        auto & _in = std::cin;
        auto & _out = std::cout;

        _out << "Glisp lisp parser\n";
        _out << "Type an expression...or [q or Q] to quit\n\n";

        while (true) {
            _out << "> ";
            auto str = glisp::read(_in);

            if (str.empty() || str[0] == 'q' || str[0] == 'Q') {
                break;
            } else {
                auto ast = glisp::eval(str);
                glisp::print(ast, _out);
                _out << endl;
            }
        }
    }
}; // namespace glisp


int main(int argc, char* argv[]) {
    using namespace std;

    using namespace ast2;

    if (false) {

    } else {

        if (argc > 1) {
            ifstream t(argv[1]);
            string str(
                (istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
            auto ast = glisp::eval(str);
            glisp::print(ast, cout);

        } else {
            glisp::repl();
        }
    }

    return 0;
}
