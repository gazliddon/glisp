#ifndef RUN_H_TB7LSZMD
#define RUN_H_TB7LSZMD

#include "ast.h"
#include "ast2.h"
#include <vector>

#include <boost/variant.hpp>

enum truthy_t
{
    True,
    False,
};

enum syntax_t
{
    If,
    And,
    Or,
    Define,
    Quote,
};

struct env_t;
struct gsymbol_t;
struct error_t;
struct nil_t;
struct proc_t;

struct gsymbol_t {
    std::string mName;
};

template <typename T>
struct proc_t_template {
    unsigned mArrity;
    std::function<T(env_t, std::vector<T>)> mFunc;

    proc_t_template() {
    }

    proc_t_template(std::function<T(env_t, std::vector<T>)> _func)
        : mFunc(_func) {
    }
};

struct error_t {};
struct nil_t {};

template <typename T>
struct list_t_template : public std::vector<T> {
    list_t_template(){};

    list_t_template(std::vector<T> _v) {
        *this = _v;
    };
};

using value_t = boost::make_recursive_variant<double,
    std::string,
    double,
    char,
    gsymbol_t,
    error_t,
    nil_t,
    truthy_t,
    syntax_t,
    proc_t_template<boost::recursive_variant_>,
    list_t_template<boost::recursive_variant_>>::type;

struct env_t {
    value_t const& lookup(gsymbol_t const& _sym) const {
        assert(false);
    }

    value_t const& lookup(
        gsymbol_t const& _sym, gsymbol_t const& _default) const {
        assert(false);
    }
};

using procedure_t = proc_t_template<value_t>;
using list_t      = list_t_template<value_t>;

void test();

#endif /* end of include guard: RUN_H_TB7LSZMD */
