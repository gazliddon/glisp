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
    ast::val low_rest(I _begin, I _end, size_t _size) {
        if (_size > 1) {
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

    double try_this(Evaluator& _e, double a, double b) {
        return 1.0;
    }

    ast::val get_symbols(Evaluator& _e, std::vector<ast::val> const&) {
        ast::map ret;

        for (auto& p : _e.mEnv) {
            map_entry me;
            me.mKey   = p.first;
            me.mValue = p.second;
            ret.mHashMap.push_front(me);
        }

        return ast::val(ret);
    }

    ast::val get(Evaluator& _e, std::vector<ast::val> const& _args) {

        if (auto p = _args[0].get_val<map>()) {
            return p->get(_args[1]);
        } else {
            cout << "Not a map!" << endl;
        }

        return ast::val();
    }

    ast::val str(Evaluator& _e, std::vector<ast::val> const& _args) {
        std::string ret = "";

        for (auto& a : _args) {
            ret += glisp::to_string(a);
        }

        return ast::val(ret);
    }

    ast::val pair_to_vec(ast::val const& a, ast::val const& b) {
        ast::vector ret;
        ret.mForms = { a, b };
        return ast::val(ret);
    }

    ast::val first(Evaluator& _e, std::vector<ast::val> const& _args) {

        auto& arg = _args[0];

        if (auto p = arg.get_val<ast::sexp>()) {
            return low_first(p->mForms);
        }

        if (auto p = arg.get_val<ast::vector>()) {
            return low_first(p->mForms);
        }

        if (auto p = arg.get_val<ast::map>()) {
            if (!p->mHashMap.empty()) {
                auto const& e = p->mHashMap.front();
                return pair_to_vec(e.mKey, e.mValue);
            }
        }

        return val();
    }

    ast::val rest(Evaluator& _e, std::vector<ast::val> const& _args) {

        auto& arg = _args[0];

        if (auto p = arg.get_val<ast::sexp>()) {
            return low_rest(
                p->mForms.begin(), p->mForms.end(), p->mForms.size());
        }

        if (auto p = arg.get_val<ast::vector>()) {
            return low_rest(
                p->mForms.begin(), p->mForms.end(), p->mForms.size());
        }

        if (auto p = arg.get_val<ast::map>()) {
            if (p->mHashMap.size() > 1) {
                sexp wrapped;
                auto& ret = wrapped.mForms;

                auto b = p->mHashMap.begin();
                auto e = p->mHashMap.end();
                b++;
                while (b != e) {
                    ret.push_back(pair_to_vec(b->mKey, b->mValue));
                    b++;
                }
                return val(wrapped);
            }
        }

        return val();
    }
    ast::val get_meta(Evaluator& _e, std::vector<ast::val> const& _args) {
        if (auto p = _args[0].get_val<lambda>()) {
            if (p->mDocString) {
                ast::map map;
                auto key = keyword { .mSym = symbol("doc") };
                map.add(val(key), val(*(p->mDocString)));
                return val(map);
            }
        }
        return val();
    }

    ast::val slurp(Evaluator& _e, std::vector<ast::val> const& _args) {

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
    ast::val concat(Evaluator& _e, std::vector<ast::val> const& _args, size_t idx_val, size_t idx_seq ) {
        auto ret = ast::val();

        sexp the_list;

        auto is_nil  = _args[idx_seq].get_val<ast::nil>();
        auto is_sexp = _args[idx_seq].get_val<ast::sexp>();

        if (is_nil || is_sexp) {
            if (is_sexp) {
                the_list = *is_sexp;
            }

            the_list.mForms.push_back(_args[idx_val]);
            return ast::val(the_list);
        } else {
            // TBD is an error
            return ast::val();
        }
    }
    ast::val conj(Evaluator& _e, std::vector<ast::val> const& _args) {
        return concat(_e, _args, 1, 0);
    }
    ast::val cons(Evaluator& _e, std::vector<ast::val> const& _args) {
        return concat(_e, _args, 0, 1);
    }

    ast::val puts(Evaluator& _e, std::vector<ast::val> const& _args) {
        std::cout << glisp::to_string(_args[0]);
        return ast::val();
    }

    ast::val equal(Evaluator& _e, std::vector<ast::val> const& _args) {
        auto const& a = _args[0];
        auto const& b = _args[1];
        return ast::val(a.var == b.var);
    }

    auto read_fn = [](Evaluator& _e, std::vector<ast::val> const& _args) {
        auto x   = _args[0].get_val<std::string>();
        auto ret = glisp::read(*x);
        return ast::val(ret.mAst);
    };

    auto eval_fn = [](Evaluator& _e, std::vector<ast::val> const& _args) {
        ast::program p { .mForms = _args };
        return _e.eval(p);
    };

    ast::val include_fn(Evaluator& _e, std::vector<ast::val> const& _args) {
        if (auto pfn = _args[0].get_val<std::string>()) {
            auto text = slurp(_e, { _args[0] });
            auto ast  = read_fn(_e, { text });
            return _e.eval(ast);
        }
        return val();
    }

    ast::val include(ast::Evaluator& _e, std::string const& _fileName) {
        return include_fn(_e, { val(_fileName) });
    }

    void add_natives(ast::Evaluator& evaluator) {

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

        evaluator.add_native_function("meta", get_meta, 1);
        evaluator.add_native_function("puts", puts, 1);
        evaluator.add_native_function("str", str, 1);
        evaluator.add_native_function("include", include_fn, 1);
        evaluator.add_native_function("symbols", get_symbols, 0);
        evaluator.add_native_function("conj", conj, 2);
        evaluator.add_native_function("cons", cons, 2);
        evaluator.add_native_function("slurp", slurp, 1);
        evaluator.add_native_function("read", read_fn, 1);
        evaluator.add_native_function("eval", eval_fn, 1);
        evaluator.add_native_function("=", equal, 2);
        evaluator.add_native_function("first", first, 1);
        evaluator.add_native_function("rest", rest, 1);
        evaluator.add_native_function("get", get, 2);
    }
}

