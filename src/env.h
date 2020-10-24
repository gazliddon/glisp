#ifndef ENV_H_UCLWRIVL
#define ENV_H_UCLWRIVL

#include <iostream>
#include <map>

#include "ast.h"

class cSeqInterface {
    public:
    virtual ast::val first() const = 0;
    virtual ast::sexp rest() const = 0;
    virtual bool as_bool() const =0;
    ast::val conj(ast::val const & _val);
};

namespace ast {

    using env_t = immer::map<std::string, val>;
    void dump(env_t const& _env, std::ostream& _out);

    env_t addEntry(env_t const & _env, std::string const & _name, ast::val const & _val);
    ast::val findEntry(env_t const & _env, std::string const &);
}

#endif /* end of include guard: ENV_H_UCLWRIVL */
