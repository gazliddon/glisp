///////////////////////////////////////////////////////////////////////////////
//#define BOOST_SPIRIT_X3_DEBUG

#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>

#include "grammar.h"
#include "printer.h"

#include "env.h"
#include "eval.h"
#include "run.h"

#include "errors.h"

// I need to walk my ast
//

namespace glisp {

    template <typename T>
    ast::val twin_op(ast::env_t e,
        std::vector<ast::val> const& _args,
        std::function<T(T const &, T const&)>&& _func) {
        assert(_args.size() == 2);
        auto a0 = _args[0].get_val<T>();
        auto a1 = _args[1].get_val<T>();
        return ast::val(_func(*a0, *a1));
    }

    ast::val println(ast::env_t e, std::vector<ast::val> const& _args) {

        if (_args[0].is_atom()) {
            ast::print(_args[0], std::cout);
        } else {
            std::cout << "Fuck knows" << std::endl;
        }
        return ast::val(ast::list());
    }

    ast::val add(ast::env_t e, std::vector<ast::val> const& _args) {
        return twin_op<double>(e, _args, [](auto a, auto b) {
                return a + b;
                });
    }

    ast::val sub(ast::env_t e, std::vector<ast::val> const& _args) {
        return twin_op<double>(e, _args, [](auto a, auto b) {
                return a - b;
                });
    }

    ast::val mul(ast::env_t e, std::vector<ast::val> const& _args) {
        return twin_op<double>(e, _args, [](auto a, auto b) {
                return a * b;
                });
    }

    ast::val div(ast::env_t e, std::vector<ast::val> const& _args) {
        return twin_op<double>(e, _args, [](auto a, auto b) {
                return a / b;
                });
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

        auto func = [](env_t, int) -> ast::val { assert(false); };

        evaluator.add_native_function("println", println, 1);

        evaluator.add_twin_op<double>("+", [](auto a, auto b) { return a + b; });
        evaluator.add_twin_op<double>("-", [](auto a, auto b) { return a - b; });
        evaluator.add_twin_op<double>("*", [](auto a, auto b) { return a * b; });
        evaluator.add_twin_op<double>("/", [](auto a, auto b) { return a / b; });

        while (true) {

            try {
                _out << "> ";
                auto str = glisp::read(_in);

                if (str[0] == 'q' || str[0] == 'Q') {
                    break;
                } else if (str[0] == 's') {
                    dump(evaluator.mEnv, _out);
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
