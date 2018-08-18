#include "ast2.h"

namespace ast2 {

    list_t list_t::cdr() const {
        assert(!mData.empty());
        return list_t {
            std::vector<value_t>(mData.begin()+1, mData.end())
        };
    }

    value_t list_t::car() const {
        assert(!mData.empty());
        return mData.front();
    }

}
