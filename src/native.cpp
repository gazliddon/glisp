#include "native.h"
#include "reader.h"
#include "tostring.h"
#include <fstream>

#include <boost/tuple/tuple.hpp>
#include <spdlog/spdlog.h>

#include <tuple>

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

    double try_this(ast::env_t e, double a, double b) {
        return 1.0;
    }

    ast::val get_symbols(ast::env_t e, std::vector<ast::val> const&) {
        ast::map ret;

        for (auto& p : e) {
            map_entry me;
            me.mKey   = p.first;
            me.mValue = p.second;
            ret.mHashMap.push_front(me);
        }

        return ast::val(ret);
    }

    ast::val get(ast::env_t e, std::vector<ast::val> const& _args) {

        if (auto p = _args[0].get_val<map>()) {
            return p->get(_args[1]);
        } else {
            cout << "Not a map!" << endl;
        }

        return ast::val();
    }

    ast::val str(ast::env_t e, std::vector<ast::val> const& _args) {
        std::stringstream ret;

        for (auto& a : _args) {
            glisp::output_string(ret, a);
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

        if (auto p = arg.get_val<ast::map>()) {
            if (!p->mHashMap.empty()) {
                auto const & e = p->mHashMap.front();
                ast::vector ret;
                ret.mForms = {e.mKey, e.mValue};
                return ast::val(ret);
            }
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

        sexp the_list;

        auto is_nil  = _args[0].get_val<ast::nil>();
        auto is_sexp = _args[0].get_val<ast::sexp>();

        if (is_nil || is_sexp) {
            if (is_sexp) {
                the_list = *is_sexp;
            }

            the_list.mForms.push_back(_args[1]);
            return ast::val(the_list);
        } else {
            // TBD is an error
            return ast::val();
        }
    }

    ast::val puts(ast::env_t e, std::vector<ast::val> const& _args) {
        glisp::output_string(std::cout, _args[0]);
        return ast::val();
    }

    ast::val equal(ast::env_t e, std::vector<ast::val> const& _args) {
        auto const& a = _args[0];
        auto const& b = _args[1];
        return ast::val(a.var == b.var);
    }

    static constexpr auto double_add = [](auto a, auto b) { return a + b; };

    ast::val do_inlude(ast::env_t e, std::vector<ast::val> const& _args) {
        return ast::val();
    }

    void add_natives(ast::Evaluator& evaluator) {

        evaluator.add_native_function("puts", puts, 1);
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

        auto read_fn = [](ast::env_t e, std::vector<ast::val> const& _args) {
            auto x   = _args[0].get_val<std::string>();
            auto ret = glisp::read(*x);
            return ast::val(ret);
        };

        auto eval_fn
            = [&evaluator](ast::env_t e, std::vector<ast::val> const& _args) {
                  ast::program p { .mForms = _args };
                  return evaluator.eval(p);
              };

        evaluator.add_native_function("symbols", get_symbols, 0);
        evaluator.add_native_function("conj", conj, 2);
        evaluator.add_native_function("slurp", slurp, 1);
        evaluator.add_native_function("read", read_fn, 1);
        evaluator.add_native_function("eval", eval_fn, 1);
        evaluator.add_native_function("=", equal, 2);
        evaluator.add_native_function("first", first, 1);
        evaluator.add_native_function("rest", rest, 1);
        evaluator.add_native_function("get", get, 2);
    }

    ast::val include(ast::Evaluator& evaluator, std::string const& _fileName) {
        auto cmd = fmt::format("(eval(read (slurp \"{}\")))", _fileName);
        auto ast = read(cmd);
        return evaluator.eval(ast);
    }
}

