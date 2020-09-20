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

#include "errors.h"

// I need to walk my ast
//

namespace glisp {
    ast::val println(ast::env_t const& e, ast::val const& _v) {
        if (_v.is_atom()) {
            ast::print(_v, std::cout);
        } else {
            std::cout << "Fuck knows" << std::endl;
        }
        return ast::val(ast::list());
    }

    void test_func(void) {
        std::cout << "GELLO!" << std::endl;
    }

    using std::cout;
    using std::endl;

    std::string read(std::istream& _in = std::cin) {
        std::string ret;
        std::getline(_in, ret);
        return ret;
    }

    ast::program read(std::string const& _str) {

        namespace x3 = boost::spirit::x3;

        using x3::ascii::space_type;
        ast::program ast;

        space_type space;
        using x3::with;

        using x3::error_handler_tag;

        using iterator_type = std::string::const_iterator;

        auto iter = _str.begin(), end = _str.end();

        using error_handler_type = x3::error_handler<iterator_type>;
        error_handler_type error_handler(iter, end, std::cerr);

        auto const parser = with<error_handler_tag>(
            std::ref(error_handler))[grammar::program];

        bool r = phrase_parse(iter, end, parser, space, ast);

        if (r && iter == end) {

        } else {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
            cout << *iter << "\n";
        }

        return ast;
    }

    void repl() {
        auto& _in  = std::cin;
        auto& _out = std::cout;

        _out << "Glisp lisp parser\n";
        _out << "Type an expression...or [q or Q] to quit\n\n";

        ast::Evaluator evaluator;

        evaluator.mEnv.add_native_func("println", test_func, 1);

        /* evaluator.mEnv.add( */
        /*         "println", */
        /*         ast::native_function { */
        /*             .mNumOfArgs = 1, */
        /*             .mFunc = println */
        /*         } */
        /*         ); */

        while (true) {

            try {
                _out << "> ";
                auto str = glisp::read(_in);

                if (str[0] == 'q' || str[0] == 'Q') {
                    break;
                } else if (str[0] == 's') {
                    evaluator.mEnv.dump(_out);

                } else {
                    auto ast = glisp::read(str);
                    /* ast::print(ast, _out); */
                    auto res = evaluator.eval(ast);

                    _out << "-> ";
                    ast::print(res, _out);
                }
            } catch (boost::spirit::x3::expectation_failure<char const*>& e) {
                _out << "ERROR " << e.what();
            }
        }
    }

}; // namespace glisp

int main(int argc, char* argv[]) {
    using namespace std;

    using namespace ast;

    if (false) {

    } else {

        if (argc > 1) {
            ifstream t(argv[1]);

            string str(
                (istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

            auto ast = glisp::read(str);
            ast::print(ast, cout);

        } else {
            glisp::repl();
        }
    }

    return 0;
}
