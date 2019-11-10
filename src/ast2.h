#ifndef AST2_H_L6QKBOEP
#define AST2_H_L6QKBOEP

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

namespace ast2 {
    namespace x3 = boost::spirit::x3;

    using x3::forward_ast;

    struct env_t;

    struct error_t {};

    struct nil_t : x3::position_tagged{};

    struct symbol_t : x3::position_tagged {
        std::string mName;
    };

    struct proc_t;
    struct list_t;

    struct truthy_t : x3::position_tagged {
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
                         char>, x3::position_tagged{
        using base_type::base_type;
        using base_type::operator=;

        value_t() {
        }

        template <typename T>
        value_t(T&& _val)
            : base_type(std::move(_val)) {
        }
    };

    struct list_t : x3::position_tagged {

        std::vector<value_t> mData;
    };

    struct proc_t : x3::position_tagged {
        unsigned mNargs;
        std::function<value_t(env_t& _env, list_t const& args)> mFunc;
    };


    struct if_t : x3::position_tagged{
        value_t mPred;
        value_t mA;
        value_t mB;
    };

    struct and_t : x3::position_tagged{
        list_t mArgs;
    };

    struct or_t : x3::position_tagged{
        list_t mArgs;
    };

    struct define_t : x3::position_tagged{
        symbol_t mSym;
        value_t mVal;
    };

    struct quote_t : x3::position_tagged{
        value_t mArg;
    };

    using boost::fusion::operator<<;
}

#endif /* end of include guard: AST2_H_L6QKBOEP */
