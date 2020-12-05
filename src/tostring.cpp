#include "tostring.h"
#include <array>
#include <boost/mp11/mpl.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <spdlog/spdlog.h>

#include "utils/cstringbuilder.h"

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

        std::string operator()( float  _val) {
            return "float";
        }

        std::string operator() (int8_t _val) {
            return "i8";

        }
        std::string operator() (uint8_t _val) {
            return "u8";

        }
        std::string operator() (int16_t _val) {
            return "i16";

        }
        std::string operator() (uint16_t _val) {
            return "u16";
        }

        std::string operator() (int32_t _val) {
            return "i32";
        }

        std::string operator() (uint32_t _val) {
            return "u32";
        }

        std::string operator() (int64_t _val) {
            return "i64";
        }

        std::string operator() (uint64_t _val) {
            return "u64";
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

    struct cToString : public boost::static_visitor<void> {
        ast::Evaluator const& mEval;

        utils::cStringBuilder mOut;
        bool mScopedSymbols;

        cToString(ast::Evaluator const& _eval, bool _withTypes = false)
            : mEval(_eval) {
            mScopedSymbols = false;
        }

        std::string build(ast::val const& _val) {
            render(_val);
            return mOut.build();
        }


        void operator() (int8_t _val) {
            mOut.print("{}", _val);

        }
        void operator() (uint8_t _val) {
            mOut.print("{}", _val);

        }
        void operator() (int16_t _val) {
            mOut.print("{}", _val);

        }
        void operator() (uint16_t _val) {
            mOut.print("{}", _val);
        }

        void operator() (int32_t _val) {
            mOut.print("{}", _val);
        }

        void operator() (uint32_t _val) {
            mOut.print("{}", _val);
        }

        void operator() (int64_t _val) {
            mOut.print("{}", _val);
        }

        void operator() (uint64_t _val) {
            mOut.print("{}", _val);
        }

        void operator()(bool const& _val) {
            std::string val_str;
            if (_val) {
                val_str = "true";
            } else {
                val_str = "false";
            }
            mOut.print(val_str);
        }


        void operator()(double _v) {
            mOut.print("{}", _v);
        }

        void operator()(ast::symbol_t const& _v) {

            if (mScopedSymbols)
                mOut.print(mEval.symbolToName(_v));
            else
                mOut.print(mEval.symbolToUnscopedName(_v));
        }

        void operator()(ast::keyword const& _v) {
            mOut.print(":");
            render(_v.mSym);
        }

        void operator()(std::string const& _v) {
            mOut.print(_v);
        }

        void operator()(ast::hint const& _v) {
            mOut.print("^");
            render(_v.mSym);
        }

        void operator()(ast::nil const& _v) {
            mOut.print("nil");
        }
        void operator()(char const* _v) {
            mOut.print(_v);
        }


        void operator()(char _v) {
            mOut.print("{}", _v);
        }

        void operator()(ast::set const& _v) {
            mOut.print(":");
            renderBraced(_v.mForms);
        }

        void operator()(ast::vector const& _val) {
            renderVector(_val.mForms);
        }

        void operator()(ast::map const& _val) {
            renderBraced(_val.mHashMap);
        }

        void operator()(ast::meta const& _val) {
            mOut.print("^{:meta \"TBD\"}");
        }

        void operator()(ast::map_entry const& _val) {
            render(_val.mKey);
            render(" : ");
            render(_val.mValue);
        }

        void operator()(ast::args const& _val) {

            render("[");

            renderCollection(_val.mArgs.mForms);
            if (_val.mExtra) {
                render(" & ");
                render(*_val.mExtra);
            }

            render("]");
        }

        void operator()(ast::lambda const& _val) {
            render("(fn ");
            render(_val.mArgs);
            mOut.indent().cr();

            if (_val.mDocString) {
                render(*_val.mDocString);
                mOut.cr();
            }

            render(_val.mBody);
            mOut.deIndent();
            render(")");
        }

        void operator()(ast::native_function const& _val) {
            mOut.print("#[fn {} :args {}]", 0xabcdef, _val.mNumOfArgs);
        }

        void operator()(ast::sexp const& _val) {
            renderList(_val.mForms);
        }

        void operator()(ast::program const& _val) {
            renderCollection(_val.mForms, "", "", "\n");
        }

        void operator()(ast::bindings const& _val) {
            renderVector(_val.mBindings);
        }

        void operator()(ast::pair const& _val) {
            render(_val.mFirst);
            render(_val.mSecond);
        }

        void operator()(ast::let const& _val) {
            render("(let ");
            render(_val.mBindings);
            mOut.indent().cr();
            render(_val.mBody);
            mOut.deIndent();
        }

        void operator()(ast::macro const& _val) {
            render("(defmacro ");
            render(_val.mSym);
            render(_val.mArgs);
            render(_val.mVal);
            render(")");
        }

        template <typename T>
        void render(T const& _val) {
            return operator()(_val);
        }

        void operator()(ast::define const& _val) {
            render("(def ");
            render(_val.mSym), render(_val.mVal);
            render(")");
        }
        void operator()(ast::unbound const&) {
            render("[UNBOUND]");
        }

        template <typename X>
        void operator()(X const& _val) {
            render(demangle(_val) + "error!");
        }

        void operator()(ast::val const& _val) {
            boost::apply_visitor(*this, _val);
        }

        template <typename T>
        void renderCollection(T const& _col,
            char const* _pre         = "",
            char const* _post        = "",
            char const* _intersperse = " ") {

            mOut.print(_pre);

            auto b = _col.begin();
            auto e = _col.end();
            while (b != e) {
                auto& elem = *b++;
                render(elem);
                if (b != e) {
                    render(_intersperse);
                }
            }
            mOut.print(_post);
        }

        template <typename T>
        void renderList(T const& _col) {
            renderCollection(_col, "(", ")");
        }

        template <typename T>
        void renderBraced(T const& _col) {
            renderCollection(_col, "{", "}");
        }

        template <typename T>
        void renderVector(T const& _col) {
            renderCollection(_col, "[", "]");
        }

        template <typename X>
        void operator()(boost::spirit::x3::forward_ast<X> const& _val) {
            operator()(_val.get());
        }

        void render(ast::val const& _v) {
            return (*this)(_v);
        }
    };

    std::string to_string(
        ast::Evaluator const& _eval, ast::val const& _val, bool _withTypes) {
        cToString to_str(_eval, _withTypes);
        return to_str.build(_val);
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
