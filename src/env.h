#ifndef ENV_H_UCLWRIVL
#define ENV_H_UCLWRIVL

#include "ast.h"
#include <iostream>

#include <map>


namespace ast {
    void dump(env_t const & _env, std::ostream & _out);

    env_t add_native_function(env_t _env, std::string const& _name,
        std::function<val(env_t, std::vector<val> const&)> && _func,
        int _nargs);
}

#endif /* end of include guard: ENV_H_UCLWRIVL */
