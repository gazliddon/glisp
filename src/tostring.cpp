#include "tostring.h"
#include <spdlog/spdlog.h>

namespace glisp {

    class cToString : public boost::static_visitor<std::string> {
    public:
        typedef std::string result_type;


        cToString()
            {
        }

        std::string operator()(bool const& _val) const {
            if (_val) {
                return "true";
            } else {
                return "false";
            }
        }

        std::string operator()(ast::symbol const& _v) const {
            return _v.mName;
        }

        std::string operator()(ast::keyword const& _v) const {
            return fmt::format(":{}", _v.mSym.mName);
        }

        std::string operator()(std::string const& _v) const {
            return _v;
        }

        std::string operator()(ast::hint const& _v) const {
            return fmt::format("^{}", _v.mSym.mName);
        }

        std::string operator()(ast::nil const&) const {
            return "nil";
        }

        std::string operator()(double _v) const {
            return fmt::format("{}", _v);
        }

        std::string operator()(char _v) const {
            return fmt::format("{}", _v);
        }

        std::string operator()(ast::set const& _set) const {
            return fmt::format("#{}", renderBraced(_set.mForms));
        }

        std::string operator()(ast::vector const& _vector) const {
            return renderVector(_vector.mForms);
        }

        std::string operator()(ast::map const& _map) const {
            return renderBraced(_map.mHashMap);
        }

        std::string operator()(ast::meta const& _value) const {
            return "^{:meta \"TBD\"}";
        }

        std::string operator()(ast::map_entry const& _v) const {
            return fmt::format("{} : {}" , render(_v.mKey), render(_v.mValue));
        }

        std::string renderArgs(ast::lambda const & _v) const {
            std::string finalArg;

            if (_v.mFinalArg) {
                finalArg = fmt::format(" & {}", _v.mFinalArg->mName);
            }

            return fmt::format("[{}{}]",  renderCollection(_v.mArgs), finalArg);
        }

        std::string operator()(ast::lambda const&_v) const {
            return fmt::format("(fn {} {})" , renderArgs(_v), render(_v.mBody));
        }

        std::string operator()(ast::native_function const& _lambda) const {
            return fmt::format("{ :fn {} :args {} }" , 0xabcdef, _lambda.mNumOfArgs);
        }

        std::string operator()(ast::sexp const& _v) const {
            return renderList(_v.mForms);
        }

        std::string operator()(ast::program const& _program) const {
            return "PRG";
        }

        std::string operator()(ast::arg const& _v) const {
            return fmt::format("{} {}" , _v.mSymbol.mName, render(_v.mVal));
        }

        std::string operator()(ast::let const& _v) const {
            return fmt::format("(let {} {})", renderVector(_v.mArgs), render(_v.mBody));
        }

        std::string operator()(ast::macro const& _v) const {
            return fmt::format("(defmacro {} {} {})", render(_v.mSym), renderVector(_v.mArgs.mForms), render( _v.mVal ));
        }

        template <typename T>
        std::string render(T const& _val) const {
            return operator()(_val);
        }

        std::string operator()(ast::define const& _v) const {
            return fmt::format("(define {} {})", _v.mSym.mName, render(_v.mVal));
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
                str+= render(*b++);
                if (b != e) {
                    str += _intersperse;
                }
            }
            return fmt::format("{}{}{}", _pre, str,_post);
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
    };

    std::string to_string(ast::val const& _val) {
        cToString to_str;
        return to_str.render(_val);
    }
}
