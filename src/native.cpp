#include "native.h"
#include "printer.h"
#include "reader.h"
#include <fstream>

#include <spdlog/spdlog.h>

namespace glisp {
    using namespace ast;

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
            auto ret_wrapped = ast::sexp(ret);
            return ast::val(ret_wrapped);
        } else {
            return ast::val(ast::nil());
        }
    }

    ast::val str(ast::env_t e, std::vector<ast::val> const& _args) {
        std::stringstream ret;

        for (auto& a : _args) {
            ast::print(a, ret);
        }

        return ast::val(ret.str());
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

    static constexpr auto double_add = [](auto a, auto b) { return a + b; };

    ast::val read(ast::Evaluator& valuator, std::string a) {
        auto read_ast = glisp::read(a);
        return ast::val(read_ast);
    }

    void add_natives(ast::Evaluator& evaluator) {

        evaluator.add_native_function("println", println, 1);
        evaluator.add_native_function("str", str, 1);

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
                  auto ret = glisp::read(evaluator, *x);
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

    void include(ast::Evaluator & evaluator, std::string const & _fileName) {
        auto cmd = fmt::format("(eval(read (slurp \"{}\")))", _fileName);
        auto ast = read(cmd);
        evaluator.eval(ast);
    }
}

