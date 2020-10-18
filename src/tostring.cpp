#include "tostring.h"

namespace glisp {

    class cToString : public boost::static_visitor<void> {
        public:
            typedef void result_type;

        std::ostream& mOut;

        cToString(std::ostream& _out, ast::val const& _val)
            : mOut(_out) {
            boost::apply_visitor(*this, _val);
        }

        void operator()(bool const& _val) const {
            if (_val) {
                mOut << "true";
            } else {
                mOut << "false";
            }
        }

        void operator()(ast::symbol const& _v) const {
            mOut << _v.mName;
        }

        void operator()(ast::keyword const& _v) const {
            mOut << ':' << _v.mSym.mName;
        }

        void operator()(std::string const& _v) const {
            mOut << _v;
        }

        void operator()(ast::hint const& _hint) const {
            mOut << "TBD HINT";
        }

        void operator()(ast::nil const&) const {
            mOut << "nil";
        }

        void operator()(double _v) const {
            mOut << _v;
        }

        void operator()(char _v) const {
            mOut << _v;
        }

        void operator()(ast::set const& _set) const {
            mOut<<"#";
            renderBraced(_set.mForms);
        }

        void operator()(ast::vector const& _vector) const {
            renderVector(_vector.mForms);
        }

        void operator()(ast::map const& _map) const {
            renderBraced(_map.mHashMap);
        }

        void operator()(ast::meta const& _value) const {
            mOut << "TBD META";
        }

        void operator()(ast::map_entry const& _map_entry) const {
            render(_map_entry.mKey);
            mOut << " : ";
            render(_map_entry.mValue);
        }

        void operator()(ast::lambda const& _lambda) const {
            mOut << "TBD LAMBDA";
        }

        void operator()(ast::native_function const& _lambda) const {
            mOut << "(fn [" << _lambda.mNumOfArgs << " args]";
        }

        void operator()(ast::sexp const& _v) const {
            renderList(_v.mForms);
        }

        void operator()(ast::program const& _program) const {
        }

        void operator()(ast::arg const& _val) const {
            render(_val.mSymbol);
            mOut << " ";
            render(_val.mVal);
        }

        void operator()(ast::let const& _val) const {
            assert(false);
        }

        void operator()(ast::macro const& _mac) const {
            mOut << "(defmacro " << _mac.mSym << " ";
            renderVector(_mac.mArgs.mForms);
            mOut << " ";
            render(_mac.mVal);
            mOut << ")";
        }

        template <typename T>
        void render(T const& _val) const {
            operator()(_val);
        }

        void operator()(ast::define const& _def) const {
            mOut << "(define " << _def.mSym.mName << " ";
            render(_def.mVal);
            mOut << ")";
        }

        void operator()(ast::val const& _val) const {
            boost::apply_visitor(*this, _val);
        }

        template <typename T>
        void renderCollection(T const& _col,
            char const* _pre         = "",
            char const* _post        = "",
            char const* _intersperse = " ") const {
            auto b = _col.begin();
            auto e = _col.end();
            mOut << _pre;
            while (b != e) {
                (*this)(*b++);
                if (b != e) {
                    mOut << _intersperse;
                }
            }
            mOut << _post;
        }

        template <typename T>
        void renderList(T const& _col) const {
            renderCollection(_col, "(", ")");
        }

        template <typename T>
        void renderBraced(T const& _col) const {
            renderCollection(_col, "{", "}");
        }

        template <typename T>
        void renderVector(T const& _col) const {
            renderCollection(_col, "[", "]");
        }
    };

    std::string to_string(ast::val const & _val) {
        std::strstream ret;
        output_string(ret, _val);
        return ret.str();
    }

    void output_string(std::ostream & _out, ast::val const & _val) {
        cToString to_string(_out, _val);
    }
}
