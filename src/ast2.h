#ifndef AST2_H_L6QKBOEP
#define AST2_H_L6QKBOEP

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast2 {
    namespace x3 = boost::spirit::x3;

    using x3::forward_ast;

    struct env_t;

    struct error_t {};

    struct nil_t {};

    struct symbol_t {
        std::string mName;
    };

    struct proc_t;
    struct list_t;

    struct truthy_t {
        bool mValue;
    };

    struct if_t;
    struct and_t;
    struct or_t;
    struct define_t;
    struct quote_t;



    struct value_t : public x3::variant<symbol_t,
                         truthy_t,
                         forward_ast<list_t>,
                         forward_ast<proc_t>,
                         forward_ast<if_t>,
                         forward_ast<and_t>,
                         forward_ast<or_t>,
                         forward_ast<define_t>,
                         forward_ast<quote_t>,
                         error_t,
                         nil_t,
                         std::string,
                         truthy_t,
                         double,
                         char> {
        using base_type::base_type;
        using base_type::operator=;

        value_t() {
        }

        template <typename T>
        value_t(T&& _val)
            : base_type(std::move(_val)) {
        }
    };

    struct list_t {

        std::vector<value_t> mData;

        list_t cdr() const;
        value_t car() const;

        bool empty() const {
            return mData.empty();
        }
    };

    struct proc_t {
        unsigned mNargs;
        std::function<value_t(env_t& _env, list_t const& args)> mFunc;
    };


    struct if_t {
        value_t mPred;
        value_t mA;
        value_t mB;
    };

    struct and_t {
        list_t mArgs;
    };

    struct or_t {
        list_t mArgs;
    };

    struct define_t {
        symbol_t mSym;
        value_t mVal;
    };

    struct quote_t {
        value_t mArg;
    };

}

#endif /* end of include guard: AST2_H_L6QKBOEP */
