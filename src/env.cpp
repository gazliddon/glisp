#include "env.h"
#include "printer.h"

namespace ast {

    void env_t::dump(std::ostream& _out) const {
        _out << "Symbol table" << std::endl;
        _out << "------------" << std::endl;

        for (auto const& p : mEnv) {
            _out << p.first << " = ";
            print(p.second, _out);
        }
    }
    void env_t::add_native_func(std::string const& name,
        std::function<void(void)> func,
        int numOfArgs) {

        add(name, val(native_function{func, numOfArgs}));
    }

    val const& env_t::get(std::string const& _k) const {
        auto it = mEnv.find(_k);
        assert(it != mEnv.end());
        return it->second;
    }

    val const& env_t::add(std::string const& k, val const& _val) {
        mEnv[k] = _val;
        return _val;
    }

    bool env_t::isDefined(std::string const& k) const {
        auto it = mEnv.find(k);
        return it != mEnv.end();
    }
} /*  ast2 */

