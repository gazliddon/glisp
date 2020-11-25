#include "seq.h"

namespace seq {

    std::unique_ptr<iterator_base_t> iterator_base_t::rest() const {
        auto ret = clone();
        ret->drop();
        return ret;
    }

    void iterator_base_t::drop() {
        next();
    }

    bool iterator_base_t::at_end() const {
        return remaining() != 0;
    }

}
