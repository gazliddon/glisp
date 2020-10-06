#ifndef PRINTER_H_9ABCZ08P
#define PRINTER_H_9ABCZ08P

#include "ast.h"
#include <assert.h>
#include <iostream>
#include <string>

// Ast printer
namespace ast {
    struct traverse_base {
        virtual void operator()(bool &) ;
        virtual void operator()(ast::symbol & _v) ;
        virtual void operator()(ast::keyword & _keyword) ;
        virtual void operator()(std::string & _v) ;
        virtual void operator()(ast::hint & _hint) ;
        virtual void operator()(ast::nil &) ;
        virtual void operator()(double _v) ;
        virtual void operator()(char _v) ;
        virtual void operator()(ast::set & _set) ;
        virtual void operator()(ast::list & _list) ;
        virtual void operator()(ast::vector & _vector) ;
        virtual void operator()(ast::map & _map) ;
        virtual void operator()(ast::meta & _value) ;
        virtual void operator()(ast::map_entry & _map_entry) ;
        virtual void operator()(ast::lambda & _lambda) ;
        virtual void operator()(ast::native_function & _lambda) ;
        virtual void operator()(ast::function & _func)  ;
        virtual void operator()(ast::define & _def) ;
        virtual void operator()(ast::val & _val) ;;
    };

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
        void operator()(bool const&) const;
        void operator()(ast::symbol const& _v) const;
        void operator()(ast::keyword const& _keyword) const;
        void operator()(std::string const& _v) const;
        void operator()(ast::hint const& _hint) const;
        void operator()(ast::nil const&) const;
        void operator()(double _v) const;
        void operator()(char _v) const;
        void operator()(ast::set const& _set) const;
        void operator()(ast::list const& _list) const;
        void operator()(ast::vector const& _vector) const;
        void operator()(ast::map const& _map) const;
        void operator()(ast::meta const& _value) const;
        void operator()(ast::map_entry const& _map_entry) const;
        void operator()(ast::lambda const& _lambda) const;
        void operator()(ast::native_function const& _lambda) const;
        void operator()(ast::function const& _func) const ;

        void operator()(ast::define const& _def) const {
            mOut << "(define " << _def.mSym.mName << " ";
            boost::apply_visitor(*this, _def.mVal);
            mOut << "):define";
        }

        void operator()(ast::val const& _val) const {
            boost::apply_visitor(*this, _val);
        }

        /* template<class T> */
        /*     void operator()(T const & _) const { */
        /*     mOut << "Sort this print out you lazy twat" << std::endl; */
        /*     } */

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
        void renderList(T const& _col, std::string const& _type) const {
            renderCollection(_col, "(", ")");
            mOut << ":" << _type;
        }

        template <typename T>
        void renderVector(T const& _col, std::string const& _type) const {
            renderCollection(_col, "[", "]");
            mOut << ":" << _type;
        }

        template <typename T>
        void renderCollection(T const& _col,
            char const* _pre,
            char const* _post,
            std::string const& _type) const {
            renderCollection(_col, _pre, _post);
            mOut << ":" << _type;
        }
    };

    template <typename T>
    void print(T const& _ast, std::ostream& _out = std::cout) {
        ast::printer printIt(_out);
        boost::apply_visitor(printIt, _ast);
        _out << std::endl;
    }

    void print(ast::program const& _p, std::ostream& _out = std::cout);

} // namespace ast

#endif /* end of include guard: PRINTER_H_9ABCZ08P */
