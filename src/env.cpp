#include "env.h"
#include "tostring.h"

namespace ast {

    void dump(env_t const & _env, std::ostream& _out) {
        _out << "Symbol table" << std::endl;
        _out << "------------" << std::endl;

        auto i = _env.begin();
        auto  e = _env.end();

        for (auto const& p : _env) {
            _out << p.first << " = " << glisp::to_string(p.second, true);
        }
    }

    env_t addEntry(env_t const & _env, std::string const & _name, ast::val const & _val) {
        return _env.insert({ _name, _val });
    }

}

