#ifndef PRINTER2_H_V7GLKDXR
#define PRINTER2_H_V7GLKDXR

#include "ast2.h"

#include <iostream>

namespace ast2 {

    struct Printer {
        std::ostream & mOut;


        Printer(std::ostream & _out) : mOut(_out) {
        }

        ~Printer() {
            mOut << std::endl;
            mOut.flush();
        }

        typedef void result_type;

        void operator()(double const& v) {
            mOut << v;
        }

        void operator()(list_t const & v) {
            mOut << "(" ;

            for (auto const & i :  v.mData) {
                boost::apply_visitor(*this, i);
                mOut << " ";
            }

            mOut << ")";
        }

        void  operator()(symbol_t const& _sym) {
            mOut << _sym.mName;
        }

        void  operator()(truthy_t const&) {
            assert(false);
        }

        void  operator()(proc_t const&) {
            mOut << "proc" ;
        }
        void  operator()(if_t const& _if) {
            mOut << "(if ";

            boost::apply_visitor(*this, _if.mPred);
            mOut << " ";
            boost::apply_visitor(*this, _if.mA);
            mOut << " ";
            boost::apply_visitor(*this, _if.mB);

            mOut << ")";
        }
        void  operator()(and_t const&) {
            assert(false);
        }

        void  operator()(or_t const&) {
            assert(false);
        }

        void  operator()(define_t const& _def) {
            mOut << "(define " ;

            (*this)(_def.mSym);

            mOut << " ";

            boost::apply_visitor(*this, _def.mVal);
            mOut << ")";
        }

        void  operator()(quote_t const&) {
            assert(false);
        }
        void  operator()(error_t const&) {
            assert(false);
        }
        void  operator()(nil_t const&) {
            mOut << "nil";
        }
        void  operator()(std::string const&_val) {
            mOut<<_val;
        }
        void  operator()(char const&_val) {
            mOut<<_val;
        }

    };

}


#endif /* end of include guard: PRINTER2_H_V7GLKDXR */
