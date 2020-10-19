#include "compile.h"

#include <iostream>

#include "printer.h"
#include "tostring.h"
#include "demangle.h"

namespace glisp {
    using namespace std;
    struct compiler_t : public boost::static_visitor<void> {

        template <typename T>
        void operator()(T & _val) {
            cout << "NO COMPILES " << demangle(_val) << endl;
        }

        template <typename T>
        void operator()(boost::spirit::x3::forward_ast<T> & _val) {
            ast::type_getter_t t;
            (*this)(_val.get());
        }

        void operator()(ast::program & _val) {
            cout << "block!" << endl;
            for (auto & v : _val.mForms) {
                compile(v);
            }
        }

        void operator()(ast::sexp & _val) {
            cout << "sexp" << endl;
            for (auto & v : _val.mForms) {
                compile(v);
            }
        }

        void operator()(bool & _val) {
            cout << "bool" << endl;
        }

        void compile(ast::val & _v) {
            _v.apply_visitor(*this);
        }
    };

    cCompiler::cCompiler() {
    }

    compiled_t cCompiler::compile(ast::program const& _ast) {
        compiler_t comp;

        auto p = ast::val(_ast);

        comp.compile(p);

        return {};
    }

}
