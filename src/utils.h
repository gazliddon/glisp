#ifndef UTILS_H_SRIB8Q4Z
#define UTILS_H_SRIB8Q4Z

#include "demangle.h"
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <iostream>

namespace ast {
    namespace x3 = boost::spirit::x3;

    template <typename result_type>
    struct ast_visitor_t : public boost::static_visitor<result_type> {

        template <typename X>
        void onError(X const& _val) const {
            std::cout << demangle(_val) << std::endl;
            assert(false);
        }

        template <typename X>
        result_type operator()(X const& _val) const {
            onError(_val);
            return result_type();
        }

        template <typename X>
        result_type operator()(x3::forward_ast<X> const& _val) const {
            return boost::apply_visitor(*this, _val.get());
        }

        template <typename X>
        result_type operator()(X& _val) {
            onError(_val);
            return result_type();
        }

        template <typename X>
        result_type operator()(x3::forward_ast<X>& _val) {
            return boost::apply_visitor(*this, _val.get());
        }

        template <typename X>
        result_type apply_visitor(X const& v) const {
            return boost::apply_visitor(*this, v);
        }

        template <typename X>
        result_type apply_visitor(X& v) {
            return boost::apply_visitor(*this, v);
        }
    };

}

#endif /* end of include guard: UTILS_H_SRIB8Q4Z */
