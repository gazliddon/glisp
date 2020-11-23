#include "native.h"
#include "reader.h"
#include <fstream>

#include <boost/tuple/tuple.hpp>
#include <spdlog/spdlog.h>

#include "compile.h"
#include <tuple>

namespace glisp {
    ast::val read_fn_x(ast::Evaluator& _e, std::vector<ast::val> const& _args) {
        assert(false);

    }

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

    ast::val get_symbols(Evaluator& _e, cIterator & _args) {
        ast::map ret;

        _e.enumerateBindings([&ret](ast::symbol_t const& _sym, val const& _val) {
            map_entry me;
            me.mKey   = _sym;
            me.mValue = _val;
            ret.mHashMap.push_front(me);
        });

        return val(ret);
    }

    template <typename T>
    boost::optional<T const &> get_next(cIterator & _t) {
        auto p = _t.next();
        if (p) {
            return p->get<T>();
        }
        return {};
    }

    val get(Evaluator& _e, cIterator & _args) {
        if (auto p = get_next<map>(_args)) {
            return p->get(*_args.next());
        } else {
            cout << "Not a map!" << endl;
        }
        return val();
    }

    ast::val get_type(Evaluator& _e, cIterator & _args) {
        auto t = _e.to_type_string(*_args.first());
        return val(t);
    }

    ast::val set(Evaluator& _e, cIterator & _args) {
        if (auto p = get_next<map>(_args)) {
            return p->get(*_args.first());
        } else {
            cout << "Not a map!" << endl;
        }

        return ast::val();
    }

    ast::val str(Evaluator& _e, cIterator & _args) {
        std::string ret = "";

        while(auto p = _args.next()) {
            ret += _e.to_string(*p);
        }

        return val(ret);
    }

    ast::val pair_to_vec(ast::val const& a, ast::val const& b) {
        ast::vector ret;
        ret.mForms = { a, b };
        return ast::val(ret);
    }

    ast::val first(Evaluator& _e, cIterator & _args) {

        auto& arg = *_args.first();

        if (auto p = arg.get<ast::sexp>()) {
            assert(false);
            return low_first(p->mForms);
        }

        if (auto p = arg.get<ast::vector>()) {
            assert(false);
            return low_first(p->mForms);
        }

        if (auto p = arg.get<ast::map>()) {
            assert(false);
            if (!p->mHashMap.empty()) {
                auto const& e = p->mHashMap.front();
                return pair_to_vec(e.mKey, e.mValue);
            }
        }

        return val();
    }

    ast::val rest(Evaluator& _e, cIterator & _args) {

        auto& arg = *_args.first();

        if (auto p = arg.get<ast::sexp>()) {
            return low_rest(
                p->mForms.begin(), p->mForms.end(), p->mForms.size());
        }

        if (auto p = arg.get<ast::vector>()) {
            return low_rest(
                p->mForms.begin(), p->mForms.end(), p->mForms.size());
        }

        if (auto p = arg.get<ast::map>()) {
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

    val get_meta(Evaluator& _e, cIterator & _args) {
        auto p = _args.next();

        if (auto l = p->get<lambda>()) {
            if (l->mDocString) {
                ast::map map;
                assert(false);
                /* auto key = keyword { .mSym = symbol("doc") }; */
                /* map.add(val(key), val(*(p->mDocString))); */
                return val(map);
            }
        }
        return val();
    }

    val slurp(Evaluator& _e, cIterator & _args) {
        if (auto file_name = get_next<std::string>(_args)) {
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

    val conj(Evaluator& _e, cIterator & _args) {
        assert(false);
        /* return concat(_e, _args, 1, 0); */
    }
    ast::val cons(Evaluator& _e, cIterator & _args) {
        assert(false);
        /* return concat(_e, _args, 0, 1); */
    }

    ast::val puts(Evaluator& _e, cIterator & _args) {
        std::cout << _e.to_string(*_args.first());
        return ast::val();
    }

    ast::val apply(Evaluator& _e, cIterator & _args) {
        return _e.apply(_args);
    }

    ast::val equal(Evaluator& _e, cIterator & _args) {
        auto const& a = *_args.next();
        auto const& b = *_args.next();
        return ast::val(a.var == b.var);
    }

    ast::val read_fn(Evaluator& _e, cIterator & _args) {
        auto text   = get_next<std::string>(_args);
        auto ret = _e.read(*text);
        return ast::val(ret.mAst);
    }

    auto eval_fn = [](Evaluator& _e, cIterator & _args) -> val {
        assert(false);
    };

    ast::val include_fn(Evaluator& _e, cIterator & _args) {

        if (auto pfn = _args.first()->get<std::string>()) {
            auto text = slurp(_e,_args );
            auto ast  = read_fn_x(_e, { text });
            /* cCompiler compiler; */
            /* compiler.compile(*ast.get_val<ast::program>()); */
            return _e.eval(ast);
        }
        return val();
    }

    /* ast::val include(ast::Evaluator& _e, std::string const& _fileName) { */
    /*     return include_fn(_e, { val(_fileName) }); */
    /* } */

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
        evaluator.add_native_function("apply", apply, 1);
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
        evaluator.add_native_function("type", get_type, 1);
    }
}

