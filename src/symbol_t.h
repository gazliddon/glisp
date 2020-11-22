#ifndef SYMBOL_T_H_TVDDJM0Q
#define SYMBOL_T_H_TVDDJM0Q

#include "position_tag_t.h"

namespace ast {

    struct symbol_t : position_tagged_t {
        symbol_t() = default;

        symbol_t(symbol_t const &_sym ) {
            mId = _sym.mId;
            mScope = _sym.mScope;
        }

        symbol_t(std::pair<uint64_t, uint64_t>&& _sym) {
            mScope = _sym.first;
            mId    = _sym.second;
        }

        symbol_t(std::pair<uint64_t, uint64_t> const & _sym) : symbol_t(_sym.first, _sym.second) { 
        }

        symbol_t(uint64_t _scope, uint64_t _id)
            : symbol_t(std::make_pair(_scope, _id)) {
        }

        friend bool operator==(symbol_t const& _lhs, symbol_t const& _rhs) {
            return _lhs.mId == _rhs.mId && _lhs.mScope == _rhs.mScope;
        }

        uint64_t mId;
        uint64_t mScope;
    };
}

#endif /* end of include guard: SYMBOL_T_H_TVDDJM0Q */
