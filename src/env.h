#ifndef ENV_H_UCLWRIVL
#define ENV_H_UCLWRIVL

#include "ast.h"
#include <iostream>

#include <map>

namespace ast {
    struct env_t {

    std::map<std::string, val> mEnv;
    void dump(std::ostream & _out = std::cout) const ;


        val const & get(std::string const & _k) const;

        val const & add(std::string const & k, val const & _val) ;

        bool isDefined(std::string const & k)  const ;

        void add_native_func(std::string const & name,
                std::function<void(void)> func,
                int numOfArgs
                );

    };

}

#endif /* end of include guard: ENV_H_UCLWRIVL */
