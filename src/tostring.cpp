#include "tostring.h"
#include <array>
#include <boost/mp11/mpl.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <spdlog/spdlog.h>
#include <regex>

namespace glisp {
    using namespace boost::mp11;
    namespace x3 = boost::spirit::x3;

    struct type_getter_t : public boost::static_visitor<std::string> {
        type_getter_t() {
        }

        std::string operator()(bool const&) {
            return "bool";
        }

        std::string operator()(ast::args const&) {
            return "args";
        }

        std::string operator()(ast::symbol_t const& _v) {
            return "symbol";
        }

        std::string operator()(ast::keyword const& _keyword) {
            return "keyword";
        }

        std::string operator()(std::string const& _v) {
            return "string";
        }

        std::string operator()(ast::hint const& _hint) {
            return "hint";
        }

        std::string operator()(ast::nil const&) {
            return "nil";
        }

        std::string operator()(double _v) {
            return "double";
        }

        std::string operator()(char _v) {
            return "char";
        }

        std::string operator()(ast::set const& _set) {
            return "set";
        }

        std::string operator()(ast::vector const& _vector) {
            return "vector";
        }

        std::string operator()(ast::map const& _map) {
            return "map";
        }

        std::string operator()(ast::meta const& _value) {
            return "meta";
        }

        std::string operator()(ast::map_entry const& _map_entry) {
            return "map_entry";
        }

        std::string operator()(ast::lambda const& _lambda) {
            return "lambda";
        }

        std::string operator()(ast::native_function const& _lambda) {
            return "native_function";
        }

        std::string operator()(ast::sexp const& _func) {
            return "sexp";
        }

        std::string operator()(ast::program const& _program) {
            return "program";
        }

        std::string operator()(ast::macro const& _mac) {
            return "macro";
        }

        std::string operator()(ast::define const& _def) {
            return "define";
        }

        std::string operator()(ast::bindings const&) {
            return "bindings";
        }

        std::string operator()(ast::pair const&) {
            return "pair";
        }

        std::string operator()(ast::let const&) {
            return "let";
        }

        template <typename X>
        std::string operator()(ast::forward_ast<X> const& _val) {
            return (*this)(_val.get());
        }

        template <typename X>
        std::string operator()(X const& _val) {
            return "!" + demangle(_val) + "!";
        }

        std::string operator()(ast::val const& _val) {
            return boost::apply_visitor(*this, _val);
        }
    };

    struct cToString : public boost::static_visitor<std::string> {
        ast::Evaluator const& mEval;
        cToString(ast::Evaluator const& _eval, bool _withTypes = false)
            : mEval(_eval)
            , mWithTypes(_withTypes)
            , mIndent(0) {
        }

        bool mWithTypes;
        unsigned mIndent;

        template <typename T>
        std::string format(T const& _v, std::string const& _vstr) {
            if (mWithTypes) {
                type_getter_t t;
                std::string _typeStr = t(_v);
                return fmt::format("{}:{}", _vstr, _typeStr);

            } else {
                return _vstr;
            }
        }

        std::string getIndent() const {
            return fmt::format("{:{}}", "", mIndent * 4);
        }

        std::string operator()(bool const& _val) {
            std::string val_str;
            if (_val) {
                val_str = "true";
            } else {
                val_str = "false";
            }
            return format(_val, val_str);
        }

        std::string operator()(ast::symbol_t const& _v) {
            return format(_v, mEval.symbolToName(_v));
        }

        std::string operator()(ast::keyword const& _v) {
            auto str = fmt::format(":{}", render(_v.mSym));
            return format(_v, str);
        }

        std::string operator()(std::string const& _v) {
            return format(_v, _v);
        }

        std::string operator()(ast::hint const& _v) {
            return format(_v, fmt::format("^{}", render(_v.mSym)));
        }

        std::string operator()(ast::nil const& _v) {
            return format(_v, "nil");
        }

        std::string operator()(double _v) {
            return format(_v, fmt::format("{}", _v));
        }

        std::string operator()(char _v) {
            return format(_v, fmt::format("{}", _v));
        }

        std::string operator()(ast::set const& _v) {
            return format(_v, fmt::format("#{}", renderBraced(_v.mForms)));
        }

        std::string operator()(ast::vector const& _val) {
            return format(_val, renderVector(_val.mForms));
        }

        std::string operator()(ast::map const& _val) {
            return format(_val, renderBraced(_val.mHashMap));
        }

        std::string operator()(ast::meta const& _val) {
            return format(_val, "^{:meta \"TBD\"}");
        }

        std::string operator()(ast::map_entry const& _val) {
            return format(_val,
                fmt::format("{} : {}", render(_val.mKey), render(_val.mValue)));
        }

        std::string operator()(ast::args const& _val) {
            std::string finalArg;
            if (_val.mExtra) {
                finalArg = " & " + render(*_val.mExtra);
            }

            return format(_val,
                fmt::format(
                    "[{}{}]", renderCollection(_val.mArgs.mForms), finalArg));
        }

        std::string operator()(ast::lambda const& _val) {
            auto line1
                = format(_val, fmt::format("(fn {}\n", render(_val.mArgs)));

            mIndent++;

            auto line2 = getIndent()
                + format(_val, fmt::format("{})", render(_val.mBody)));

            mIndent--;

            return line1 + line2;
        }

        std::string operator()(ast::native_function const& _val) {
            return format(_val,
                fmt::format("[fn {} :args {}]", 0xabcdef, _val.mNumOfArgs));
        }

        std::string operator()(ast::sexp const& _val) {
            return format(_val, renderList(_val.mForms));
        }

        std::string operator()(ast::program const& _val) {
            auto pre         = getIndent();
            auto intersperse = "\n";
            return format(_val,
                renderCollection(_val.mForms, pre.c_str(), "", intersperse));
        }

        std::string operator()(ast::bindings const& _val) {
            return renderVector(_val.mBindings);
        }

        std::string operator()(ast::pair const& _val) {
            return fmt::format(
                "{} {}", render(_val.mFirst), render(_val.mSecond));
        }

        std::string operator()(ast::let const& _val) {

            auto line1 = format(
                _val, fmt::format("(let {}\n", render(_val.mBindings)));

            mIndent++;

            auto line2 = getIndent()
                + format(_val, fmt::format("{})", render(_val.mBody)));

            mIndent--;

            return line1 + line2;
        }

        std::string operator()(ast::macro const& _val) {
            return format(_val,
                fmt::format("(defmacro {} {} {})",
                    render(_val.mSym),
                    render(_val.mArgs),
                    render(_val.mVal)));
        }

        template <typename T>
        std::string render(T const& _val) {
            return operator()(_val);
        }

        std::string operator()(ast::define const& _val) {
            return format(_val,
                fmt::format(
                    "(define {} {})", render(_val.mSym), render(_val.mVal)));
        }
        std::string operator()(ast::unbound const&) {
            return "[UNBOUND]";
        }

        template <typename X>
        std::string operator()(X const& _val) {
            return demangle(_val) + "error!";
        }

        std::string operator()(ast::val const& _val) {
            return boost::apply_visitor(*this, _val);
        }

        template <typename T>
        std::string renderCollection(T const& _col,
            char const* _pre         = "",
            char const* _post        = "",
            char const* _intersperse = " ") {

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
        std::string renderList(T const& _col) {
            return renderCollection(_col, "(", ")");
        }

        template <typename T>
        std::string renderBraced(T const& _col) {
            return renderCollection(_col, "{", "}");
        }

        template <typename T>
        std::string renderVector(T const& _col) {
            return renderCollection(_col, "[", "]");
        }

        template <typename X>
        std::string operator()(boost::spirit::x3::forward_ast<X> const& _val) {
            return operator()(_val.get());
        }

        std::string render(ast::val const& _v) {
            return (*this)(_v);
        }
    };

    std::string to_string(
        ast::Evaluator const& _eval, ast::val const& _val, bool _withTypes) {
        cToString to_str(_eval, _withTypes);
        return to_str(_val);
    }

    std::string to_type_string(ast::val const& _val) {
        type_getter_t t;
        return t(_val);
    }

    std::string to_string(ast::Evaluator const& _eval,
        ast::cIterator& _it,
        char const* intersperse,
        bool _withTypes) {

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
