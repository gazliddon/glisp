#include "ast.h"
#include "eval.h"

namespace ast {

    bool operator==(lambda const& _lhs, lambda const& _rhs) {
        assert(false);
    }

    bool operator==(set const& _lhs, set const& _rhs) {
        assert(false);
    }
    bool operator==(meta const& _lhs, meta const& _rhs) {
        assert(false);
    }
    bool operator==(map const& _lhs, map const& _rhs) {
        assert(false);
    }
    bool operator==(map_entry const& _lhs, map_entry const& _rhs) {
        assert(false);
    }
    bool operator==(vector const& _lhs, vector const& _rhs) {
        assert(false);
    }
    bool operator==(native_function const& _lhs, native_function const& _rhs) {
        assert(false);
    }

    bool operator==(keyword const& _lhs, keyword const& _rhs) {
        return _lhs.mSym == _rhs.mSym;
    }

    bool operator==(symbol const& _lhs, symbol const& _rhs) {
        return _lhs.mName == _rhs.mName;
    }

    bool operator==(program const& _lhs, program const& _rhs) {
        assert(false);
    }

}




