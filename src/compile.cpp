#include "compile.h"

#include <iostream>

#include "printer.h"
#include "tostring.h"
#include "demangle.h"

namespace glisp {
    using namespace std;
    struct compiler_t : public boost::static_visitor<void> {

        template <typename T>
        void operator()(T const & _val) const {
            cout << "NO COMPILES " << demangle(_val) << endl;
        }

        template <typename T>
        void operator()(boost::spirit::x3::forward_ast<T> const & _val) const {
            ast::type_getter_t t;
            (*this)(_val.get());
        }

        template <>
        void operator()(ast::program const& _val) const {
            cout << "block!" << endl;
            for (auto const & v : _val.mForms) {
                compile(v);
            }
        }

        void compile(ast::val const & _v) const {
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
