#include "keywords.h"

namespace ast {

    keyword_id_t cKeyWordStore::add(std::string const& _kw) {
        auto it = mNameToId.find(_kw);
        assert(it == mNameToId.end());
        auto ret = mKeywords.size();
        mKeywords.push_back(_kw);
        mNameToId[_kw] = ret;
        return ret;
    }

    std::string const& cKeyWordStore::get(keyword_id_t _i) const {
        auto x = static_cast<size_t>(_i);
        assert(x < mKeywords.size());
        return mKeywords[x];
    }
}
