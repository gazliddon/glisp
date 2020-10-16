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

class seq_it {

public:
    using Coll = std::vector<ast::val>;
    using It   = Coll::iterator;

    seq_it(std::vector<ast::val>& v)
        : mBegin(v.begin())
        , mEnd(v.end())
        , mIt(mBegin) {
    }

    bool next() {
        if (mIt == mEnd) {
            return false;
        } else {
            mIt++;
            return true;
        }
    }

    ast::val& operator*() const {
        return *mIt;
    }

    ast::val* operator->() const {
        return &*mIt;
    }

protected:
    It mBegin;
    It mEnd;
    It mIt;
};

namespace glisp {
    using std::cout;
    using std::endl;


    template <typename T>
    ast::val low_first(T const& _v) {
        if (_v.size() > 0) {
            return *_v.begin();
        }
        return ast::val(ast::nil());
    }

    template <class I>
    ast::val low_rest(I _begin, I _end) {
        if (_end - _begin > 1) {
            std::vector<ast::val> ret;
            _begin++;
            while (_begin != _end) {
                ret.push_back(*_begin);
                _begin++;
            }
            auto ret_wrapped = ast::sexp( ret );
            return ast::val(ret_wrapped);
        } else {
            return ast::val(ast::nil());
        }
    }

    ast::val str(ast::env_t e, std::vector<ast::val> const& _args) {
        return ast::val(ast::nil());
    }

    ast::val first(ast::env_t e, std::vector<ast::val> const& _args) {

        auto& arg = _args[0];

        if (auto p = arg.get_val<ast::sexp>()) {
            return low_first(p->mForms);
        }

        if (auto p = arg.get_val<ast::vector>()) {
            return low_first(p->mForms);
        }

        return ast::val(ast::nil());
    }

    ast::val rest(ast::env_t e, std::vector<ast::val> const& _args) {

        auto& arg = _args[0];

        if (auto p = arg.get_val<ast::sexp>()) {
            return low_rest(p->mForms.begin(), p->mForms.end());
        }

        if (auto p = arg.get_val<ast::vector>()) {
            return low_rest(p->mForms.begin(), p->mForms.end());
        }

        return ast::val(ast::nil());
    }

    ast::val slurp(ast::env_t e, std::vector<ast::val> const& _args) {

        if (auto file_name = _args[0].get_val<std::string>()) {
            std::ifstream inFile;
            inFile.open(file_name->c_str()); // open the input file
            std::stringstream strStream;
            strStream << inFile.rdbuf(); // read the file
            std::string str
                = strStream.str(); // str holds the content of the file
            return ast::val(str);
        } else {
            return ast::val(ast::nil());
        }
    }

    ast::val conj(ast::env_t e, std::vector<ast::val> const& _args) {


        auto ret = ast::val();

        if (auto p = _args[0].get_val<ast::sexp>()) {

            auto copy = *p;
            copy.conj(_args[1]);
            ret = copy;
        }

        return ret;
    }

    ast::val println(ast::env_t e, std::vector<ast::val> const& _args) {
        ast::print(_args[0], std::cout);
        return ast::val(ast::nil());
    }

    ast::val equal(ast::env_t e, std::vector<ast::val> const& _args) {
        auto const& a = _args[0];
        auto const& b = _args[1];
        return ast::val(a.var == b.var);
    }

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

    ast::val read(ast::Evaluator& valuator, std::string a) {
        auto read_ast = glisp::read(a);
        return ast::val(read_ast);
    }

    static constexpr auto double_add = [](auto a, auto b) { return a + b; };

    void add_natives(ast::Evaluator& evaluator) {

        evaluator.add_native_function("println", println, 1);

        evaluator.add_twin_op<double, double>(
            "+", [](auto a, auto b) { return a + b; });
        evaluator.add_twin_op<double, double>(
            "-", [](auto a, auto b) { return a - b; });
        evaluator.add_twin_op<double, double>(
            "*", [](auto a, auto b) { return a * b; });
        evaluator.add_twin_op<double, double>(
            "/", [](auto a, auto b) { return a / b; });

        evaluator.add_twin_op<double, double>(
            "pow", [](auto a, auto b) { return pow(a, b); });

        evaluator.add_twin_op<double, bool>(
            "<=", [](auto a, auto b) { return a <= b; });
        evaluator.add_twin_op<double, bool>(
            ">=", [](auto a, auto b) { return a >= b; });
        evaluator.add_twin_op<double, bool>(
            "<", [](auto a, auto b) { return a < b; });
        evaluator.add_twin_op<double, bool>(
            ">", [](auto a, auto b) { return a > b; });

        auto read_fn
            = [&evaluator](ast::env_t e, std::vector<ast::val> const& _args) {
                  auto x   = _args[0].get_val<std::string>();
                  auto ret = read(evaluator, *x);
                  return ret;
              };

        auto eval_fn
            = [&evaluator](ast::env_t e, std::vector<ast::val> const& _args) {
                  ast::program p { .mForms = _args };
                  return evaluator.eval(p);
              };

        evaluator.add_native_function("conj", conj, 2);
        evaluator.add_native_function("slurp", slurp, 1);
        evaluator.add_native_function("read", read_fn, 1);
        evaluator.add_native_function("eval", eval_fn, 1);
        evaluator.add_native_function("=", equal, 2);
        evaluator.add_native_function("first", first, 1);
        evaluator.add_native_function("rest", rest, 1);
    }

    void include_prelude(ast::Evaluator& evaluator) {
        auto prelude = glisp::read("(eval (read (slurp \"prelude.gl\")))");
        evaluator.eval(prelude);
    }

    void repl() {
        auto& _in  = std::cin;
        auto& _out = std::cout;

        _out << "Glisp lisp parser\n";
        _out << "Type an expression...or [q or Q] to quit\n\n";

        ast::Evaluator evaluator;
        add_natives(evaluator);

        _out << "including prelude.gl" << endl;
        include_prelude(evaluator);

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
                    auto res = evaluator.eval(ast);

                    _out << "-> ";

                    ast::print(res, _out);

                    evaluator.mEnv = evaluator.mEnv.set("*1", res);
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
