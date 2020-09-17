#ifndef ENV_H_UCLWRIVL
#define ENV_H_UCLWRIVL

#include "ast.h"
#include "printer.h"

#include <map>

namespace ast {

    struct env_t {
        void dump(std::ostream & _out = std::cout) {
            _out << "Symbol table" << std::endl;
            _out << "------------" << std::endl;

            for (auto const & p : mEnv) {
                _out << p.first << " = ";
                print(p.second, _out);
            }
        }

        std::map<std::string, val> mEnv;

        val const & get(std::string const & _k) {
            auto it = mEnv.find(_k);
            assert(it != mEnv.end());
            return it->second;
        }

        val const & add(std::string const & k, val const & _val) {
            mEnv[k] = _val;
            return _val;
        }

        bool isDefined(std::string const & k) {
            auto it = mEnv.find(k);
            return it != mEnv.end();
        }


    };
}

#endif /* end of include guard: ENV_H_UCLWRIVL */
