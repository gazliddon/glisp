#ifndef ENV_H_UCLWRIVL
#define ENV_H_UCLWRIVL

#include "ast2.h"

#include <map>

namespace ast2 {

    struct env_t {
        std::map<std::string, value_t> mEnv;

        value_t const & get(std::string const & _k) {
            auto it = mEnv.find(_k);
            assert(it != mEnv.end());
            return it->second;
        }

        void add(std::string const & k, value_t const & _val) {
            mEnv[k] = _val;
        }

        bool isDefined(std::string const & k) {
            auto it = mEnv.find(k);
            return it != mEnv.end();
        }

    };
}

#endif /* end of include guard: ENV_H_UCLWRIVL */
