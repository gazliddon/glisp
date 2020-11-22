#include "tostring.h"
#include "utils.h"
#include <array>
#include <boost/mp11/mpl.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <spdlog/spdlog.h>

namespace glisp {
    using namespace boost::mp11;
    namespace x3 = boost::spirit::x3;

    /* template <typename... A> */
    /* class cChain : boost::static_visitor<std::string> { */
    /* public: */
    /*     using list_t                   = mp_list<A...>; */
    /*     static size_t const mNumOfArgs = mp_size<list_t>(); */

    /*     cChain(std::string _intersperse = "") */
    /*         : mIntersperse(_intersperse) { */
    /*     } */

    /*     template <typename T> */
    /*     std::string operator()(T const& _val) { */
    /*         std::string ret; */
    /*         return ret; */
    /*     } */

    /*     std::string render(ast::val const& _val) { */
    /*         std::string ret; */

    /*         auto f = [&](auto renderer, bool not_last) { */
    /*             ret += renderer.render(_val); */
    /*             if (not_last) { */
    /*                 ret += mIntersperse; */
    /*             } */
    /*         }; */

    /*         mp_for_each<list_t>([&](auto out) { */
    /*             using idx_t = mp_find<list_t, decltype(out)>; */
    /*             auto idx    = idx_t(); */
    /*             f(out, idx != (mNumOfArgs - 1)); */
    /*         }); */

    /*         return ret; */
    /*     } */

    /* protected: */
    /*     std::string mIntersperse; */
    /* }; */

    struct type_getter_t : public boost::static_visitor<std::string> {
        type_getter_t() {
        }

        std::string operator()(bool const&) const {
            return "bool";
        }

        std::string operator()(ast::args const&) const {
            return "args";
        }

        std::string operator()(ast::symbol_t const& _v) const {
            return "symbol";
        }

        std::string operator()(ast::keyword const& _keyword) const {
            return "keyword";
        }

        std::string operator()(std::string const& _v) const {
            return "string";
        }

        std::string operator()(ast::hint const& _hint) const {
            return "hint";
        }

        std::string operator()(ast::nil const&) const {
            return "nil";
        }

        std::string operator()(double _v) const {
            return "double";
        }

        std::string operator()(char _v) const {
            return "char";
        }

        std::string operator()(ast::set const& _set) const {
            return "set";
        }

        std::string operator()(ast::vector const& _vector) const {
            return "vector";
        }

        std::string operator()(ast::map const& _map) const {
            return "map";
        }

        std::string operator()(ast::meta const& _value) const {
            return "meta";
        }

        std::string operator()(ast::map_entry const& _map_entry) const {
            return "map_entry";
        }

        std::string operator()(ast::lambda const& _lambda) const {
            return "lambda";
        }

        std::string operator()(ast::native_function const& _lambda) const {
            return "native_function";
        }

        std::string operator()(ast::sexp const& _func) const {
            return "sexp";
        }

        std::string operator()(ast::program const& _program) const {
            return "program";
        }

        std::string operator()(ast::macro const& _mac) const {
            return "macro";
        }

        std::string operator()(ast::define const& _def) const {
            return "define";
        }

        std::string operator()(ast::bindings const&) const {
            return "bindings";
        }

        std::string operator()(ast::pair const&) const {
            return "pair";
        }

        std::string operator()(ast::let const&) const {
            return "let";
        }

        template <typename X>
        std::string operator()(ast::forward_ast<X> const& _val) const {
            return (*this)(_val.get());
        }

        template <typename X>
        std::string operator()(X const& _val) const {
            return "!" + demangle(_val) + "!";
        }

        std::string operator()(ast::val const& _val) const {
            return boost::apply_visitor(*this, _val);
        }
    };

    struct cToString : public boost::static_visitor<std::string> {
        ast::Evaluator const& mEval;
        cToString(ast::Evaluator const& _eval, bool _withTypes = false)
            : mEval(_eval)
            , mWithTypes(_withTypes) {
        }

        bool mWithTypes;

        template <typename T>
        std::string format(T const& _v, std::string const& _vstr) const {
            if (mWithTypes) {
                type_getter_t t;
                std::string _typeStr = t(_v);
                return fmt::format("{}:{}", _vstr, _typeStr);

            } else {
                return _vstr;
            }
        }

        std::string operator()(bool const& _val) const {
            std::string val_str;
            if (_val) {
                val_str = "true";
            } else {
                val_str = "false";
            }
            return format(_val, val_str);
        }

        std::string operator()(ast::symbol_t const& _v) const {
            return format(_v, mEval.symbolToName(_v ));
        }

        std::string operator()(ast::keyword const& _v) const {
            auto str = fmt::format(":{}", render(_v.mSym));
            return format(_v, str);
        }

        std::string operator()(std::string const& _v) const {
            return format(_v, _v);
        }

        std::string operator()(ast::hint const& _v) const {
            return format(_v, fmt::format("^{}", render(_v.mSym)));
        }

        std::string operator()(ast::nil const& _v) const {
            return format(_v, "nil");
        }

        std::string operator()(double _v) const {
            return format(_v, fmt::format("{}", _v));
        }

        std::string operator()(char _v) const {
            return format(_v, fmt::format("{}", _v));
        }

        std::string operator()(ast::set const& _v) const {
            return format(_v, fmt::format("#{}", renderBraced(_v.mForms)));
        }

        std::string operator()(ast::vector const& _val) const {
            return format(_val, renderVector(_val.mForms));
        }

        std::string operator()(ast::map const& _val) const {
            return format(_val, renderBraced(_val.mHashMap));
        }

        std::string operator()(ast::meta const& _val) const {
            return format(_val, "^{:meta \"TBD\"}");
        }

        std::string operator()(ast::map_entry const& _val) const {
            return format(_val,
                fmt::format("{} : {}", render(_val.mKey), render(_val.mValue)));
        }

        std::string operator()(ast::args const& _val) const {
            std::string finalArg;
            if (_val.mExtra) {
                finalArg = " & " + render(*_val.mExtra);
            }

            return format(_val,
                fmt::format(
                    "[{}{}]", renderCollection(_val.mArgs.mForms), finalArg));
        }

        std::string operator()(ast::lambda const& _val) const {
            return format(_val,
                fmt::format(
                    "(fn {} {})", render(_val.mArgs), render(_val.mBody)));
        }

        std::string operator()(ast::native_function const& _val) const {
            return format(_val,
                fmt::format("[fn {} :args {}]", 0xabcdef, _val.mNumOfArgs));
        }

        std::string operator()(ast::sexp const& _val) const {
            return format(_val, renderList(_val.mForms));
        }

        std::string operator()(ast::program const& _val) const {
            return format(_val, renderCollection(_val.mForms));
        }

        std::string operator()(ast::bindings const& _val) const {
            return renderVector(_val.mBindings);
        }

        std::string operator()(ast::pair const& _val) const {
            return fmt::format(
                "{} {}", render(_val.mFirst), render(_val.mSecond));
        }

        std::string operator()(ast::let const& _val) const {
            return format(_val,
                fmt::format(
                    "(let {} {})", render(_val.mBindings), render(_val.mBody)));
        }

        std::string operator()(ast::macro const& _val) const {
            return format(_val,
                fmt::format("(defmacro {} {} {})",
                    render(_val.mSym),
                    render(_val.mArgs),
                    render(_val.mVal)));
        }

        template <typename T>
        std::string render(T const& _val) const {
            return operator()(_val);
        }

        std::string operator()(ast::define const& _val) const {
            return format(_val,
                fmt::format(
                    "(define {} {})", render(_val.mSym), render(_val.mVal)));
        }
        std::string operator()(ast::unbound const& ) const {
            return "[UNBOUND]";
        }

        template <typename X>
        std::string operator()(X const& _val) const {
            return demangle(_val) + "error!";
        }

        std::string operator()(ast::val const& _val) const {
            return boost::apply_visitor(*this, _val);
        }

        template <typename T>
        std::string renderCollection(T const& _col,
            char const* _pre         = "",
            char const* _post        = "",
            char const* _intersperse = " ") const {

            std::string str;

            auto b = _col.begin();
            auto e = _col.end();
            while (b != e) {
                auto& elem = *b++;
                str += render(elem);
                if (b != e) {
                    str += _intersperse;
                }
            }
            return fmt::format("{}{}{}", _pre, str, _post);
        }

        template <typename T>
        std::string renderList(T const& _col) const {
            return renderCollection(_col, "(", ")");
        }

        template <typename T>
        std::string renderBraced(T const& _col) const {
            return renderCollection(_col, "{", "}");
        }

        template <typename T>
        std::string renderVector(T const& _col) const {
            return renderCollection(_col, "[", "]");
        }

        template <typename X>
        std::string operator()(
            boost::spirit::x3::forward_ast<X> const& _val) const {
            return operator()(_val.get());
        }

        std::string render(ast::val const& _v) const {
            return (*this)(_v);
        }
    };

    std::string to_string(
        ast::Evaluator const& _eval, ast::val const& _val, bool _withTypes) {
        cToString to_str(_eval, _withTypes);
        return to_str(_val);
    }

    std::string to_type_string(ast::val const & _val) {
        type_getter_t t;
        return t(_val);
    }
    
    std::string to_string(ast::Evaluator const & _eval,
            ast::iterator_base_t& _it, char const* intersperse, bool _withTypes) {

        std::strstream x;

        bool first = true;

        while (auto p = _it.next()) {

            if (!first) {
                x << intersperse;
            }
            x << to_string(_eval, *p);

            first = false;
        }
        return x.str();
    }

}
