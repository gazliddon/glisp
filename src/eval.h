#ifndef EVAL_H_I8ZWFS1M
#define EVAL_H_I8ZWFS1M

#include "ast2.h"
#include "env.h"

namespace ast2 {

    // Visitor to evaluate this expression
    struct Evaluator {
        typedef value_t result_type;
        result_type operator()(symbol_t const&);
        result_type operator()(truthy_t const&);
        result_type operator()(list_t const&);
        result_type operator()(proc_t const&);
        result_type operator()(if_t const&);
        result_type operator()(and_t const&);
        result_type operator()(or_t const&);
        result_type operator()(define_t const&);
        result_type operator()(quote_t const&);
        result_type operator()(error_t const&);
        result_type operator()(nil_t const&);
        result_type operator()(std::string const&);
        result_type operator()(double const&);
        result_type operator()(char const&);

        value_t apply(proc_t const& _p, list_t const& _args);

        env_t mEnv;

        value_t eval(value_t const& _v);
    };

    // Visitor to return whether this
    // variant will evail to itself or
    // not
    struct does_eval {
        typedef bool result_type;
        result_type operator()(symbol_t const&) const;
        result_type operator()(truthy_t const&) const;
        result_type operator()(list_t const&) const;
        result_type operator()(proc_t const&) const;
        result_type operator()(if_t const&) const;
        result_type operator()(and_t const&) const;
        result_type operator()(or_t const&) const;
        result_type operator()(define_t const&) const;
        result_type operator()(quote_t const&) const;
        result_type operator()(error_t const&) const;
        result_type operator()(nil_t const&) const;
        result_type operator()(std::string const&) const;
        result_type operator()(double const&) const;
        result_type operator()(char const&) const;
    };

    struct to_bool {
        typedef bool result_type;

        result_type operator()(symbol_t const&) const {
            return false;
        }

        result_type operator()(truthy_t const&_v) const {
            return _v.mValue;
        }
        result_type operator()(list_t const&) const {
            return true;
        }
        result_type operator()(proc_t const&) const {
            return false;
        }
        result_type operator()(if_t const&) const {
            return false;
        }
        result_type operator()(and_t const&) const {
            return false;
        }
        result_type operator()(or_t const&) const {
            return false;
        }
        result_type operator()(define_t const&) const {
            return false;
        }
        result_type operator()(quote_t const&) const {
            return false;
        }
        result_type operator()(error_t const&) const {
            return false;
        }
        result_type operator()(nil_t const&) const {
            return false;
        }
        result_type operator()(std::string const&) const {
            return true;
        }
        result_type operator()(double const&) const {
            return false;
        }
        result_type operator()(char const&) const {
            return false;
        }
    };

    struct print_val {
        typedef std::string result_type;
        result_type operator()(symbol_t const&) const;
        result_type operator()(truthy_t const&) const;
        result_type operator()(list_t const&) const;
        result_type operator()(proc_t const&) const;
        result_type operator()(if_t const&) const;
        result_type operator()(and_t const&) const;
        result_type operator()(or_t const&) const;
        result_type operator()(define_t const&) const;
        result_type operator()(quote_t const&) const;
        result_type operator()(error_t const&) const;
        result_type operator()(nil_t const&) const;
        result_type operator()(std::string const&) const;
        result_type operator()(double const&) const;
        result_type operator()(char const&) const;
    };

    void testEval();
}

#endif /* end of include guard: EVAL_H_I8ZWFS1M */
