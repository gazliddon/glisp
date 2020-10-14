#include "env.h"
#include "printer.h"

namespace ast {

    void dump(env_t const & _env, std::ostream& _out) {
        _out << "Symbol table" << std::endl;
        _out << "------------" << std::endl;

        auto i = _env.begin();
        auto  e = _env.end();

        for (auto const& p : _env) {
            _out << p.first << " = ";
            print(p.second, _out);
        }
    }

    env_t add_native_function(env_t _env, std::string const& _name,
        std::function<val(env_t, std::vector<val> const&)> && _func,
        int _nargs) {

        native_function x {
            .mFunc      = std::move(_func),
            .mNumOfArgs = _nargs,
        };
        
       return _env.set(_name, val(x));
    }

} /*  ast2 */

