#include "env.h"

#include <immer/algorithm.hpp>

namespace ast {

    void dump(env_t const & _env, std::ostream& _out) {
        _out << "Symbol table" << std::endl;
        _out << "------------" << std::endl;

        auto const i = _env.begin();
        auto  const e = _env.end();

        immer::for_each(_env, [](auto const & x){});

        /* for(auto it = i; i != e; it++) { */
            /* _out << p.first << " = " << glisp::to_string(p.second, true); */
        /* } */
    }

    env_t addEntry(env_t const & _env, std::string const & _name, ast::val const & _val) {
        return _env.insert({ _name, _val });
    }

}

