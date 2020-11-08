#include "compile.h"

#include <stack>
#include <iostream>

#include "demangle.h"

namespace glisp {
    using namespace std;

    struct compiler_t : public boost::static_visitor<void> {

        template <typename T>
        void operator()(T & _val) {
            cout << "DOES NOT COMPILE" << demangle(_val) << endl;
        }

        template <typename T>
        void operator()(boost::spirit::x3::forward_ast<T> & _val) {
            (*this)(_val.get());
        }


        void operator()(ast::program & _val) {
            /* for (auto & v : _val.mForms) { */
            /*     compile(v); */
            /* } */
        }

        void operator()(ast::define & _val) {
            addSymbol(_val.mSym);
            compile(_val.mVal);
        }

        void operator()(ast::symbol & _val) {
            /* if(hasSymbol(_val)) { */
            /* } else { */
            /*     mUndefinedSyms[_val.mName] = &_val; */
            /* } */
        }

        void operator()(ast::sexp & _val) {
            for(auto & v : _val.mForms) {
                compile(v);
            }
        }

        void compile(ast::val & _v) {
            mValStack.push(&_v);
            _v.apply_visitor(*this);
            mValStack.pop();
        }

        void operator()(ast::lambda & _v) {
            // TODO add args to env
            (*this)(_v.mBody);
        }

        void operator()(ast::let & _v) {
            // TODO add args to env
            (*this)(_v.mBody);
        }

        void start_compile(ast::val & _v) {
            using namespace std;

            cout << "Starting compile" << endl;

            compile(_v);


            cout << "undefined syms: " << mUndefinedSyms.size() << endl;

            for (auto & p : mUndefinedSyms) {
                cout << p.first << endl;
            }
        }

        ast::val & currentVal() {
            return *mValStack.top();
        }

        bool hasSymbol(ast::symbol & _sym) const {
            /* auto it = mInternalSyms.find(_sym.mName); */
            /* return it != mInternalSyms.end(); */
            return false;
        }

        void addSymbol(ast::symbol & _sym) {
            /* mInternalSyms[_sym.mName]=&_sym; */
        }

        std::stack<ast::val *> mValStack;
        std::map<std::string, ast::symbol *> mInternalSyms;
        std::map<std::string, ast::symbol *> mUndefinedSyms;
    };

    cCompiler::cCompiler() {
    }

    compiled_t cCompiler::compile(ast::program const& _ast) {
        compiler_t comp;

        /* auto p = ast::val(_ast); */

        /* comp.start_compile(p); */

        return {};
    }

}
