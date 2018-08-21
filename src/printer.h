#ifndef PRINTER_H_9ABCZ08P
#define PRINTER_H_9ABCZ08P

#include "ast.h"
#include <assert.h>
#include <iostream>
#include <string>

// Ast printer
namespace ast {
    struct printer_base {
        typedef void result_type;
        std::ostream& mOut;
        int mIndent;
        printer_base(std::ostream& _out)
            : mOut(_out)
            , mIndent(-1) {
        }
    };

    struct printer : public printer_base {
        printer(std::ostream& _out = std::cout);

        /* void operator()(ast::val const& _v) const; */
        void operator()(ast::special const& _v) const;
        void operator()(ast::boolean const&) const;
        void operator()(ast::symbol const& _v) const;
        void operator()(ast::keyword const& _keyword) const;
        void operator()(std::string const& _v) const;
        void operator()(ast::hint const& _hint) const;
        void operator()(ast::nil const&) const;
        void operator()(double _v) const;
        void operator()(char _v) const;
        void operator()(ast::set const& _set) const;
        void operator()(ast::application const& _list) const;
        void operator()(ast::vector const& _vector) const;
        void operator()(ast::map const& _map) const;
        void operator()(ast::meta const& _value) const;
        void operator()(ast::map_entry const& _map_entry) const;
        void operator()(ast::sp_define const& _define) const;

        void operator()(ast::sp_if const& _sp_if) const {
            std::vector<val> vals = {val{ "if" }, _sp_if.mPred, _sp_if.mTrue, _sp_if.mFalse};
            renderList(vals, "special");
        }

        void operator()(ast::sp_or const& _sp_or) const {
            renderCollection(_sp_or.mVals, "(or ", ")", "special");
        }

        void operator()(ast::sp_and const& _sp_and) const {
            renderCollection(_sp_and.mVals, "(and ", ")", "special");
        }

        template <typename T>
        void renderCollection(
            T const& _col, std::string _intersperse = " ") const {
            auto b = _col.begin();
            auto e = _col.end();
            while (b != e) {
                boost::apply_visitor(*this, *b++);
                if (b != e) {
                    mOut << _intersperse;
                }
            }
        }
        template <typename T>
        void renderList(T const& _col,
            std::string const& _type) const {
            mOut << "(";
            renderCollection(_col);
            mOut << ")" << ":" << _type;
        }

        template <typename T>
        void renderCollection(T const& _col,
            std::string const& _pre,
            std::string const& _post,
            std::string const& _type) const {
            mOut << _pre;
            renderCollection(_col);
            mOut << _post << ":" << _type;
        }
    };

} // namespace ast

#endif /* end of include guard: PRINTER_H_9ABCZ08P */
