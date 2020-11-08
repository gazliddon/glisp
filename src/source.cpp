#include "source.h"

namespace ast {

    cSourceStore::cSourceStore()
        : mKey(0) {
    }

    source_key_t cSourceStore::addSourceText(
        std::string const& _text, std::string const& _fileName) {
        auto key      = mKey++;
        mSources[key] = source_text_t {
            .mText      = _text,
            .mFileName  = _fileName,
            .mSourceKey = key,
        };

        return key;
    }

    int cSourceStore::getLineNumber(annotation_t const * _t) const {
        assert(false);
    }

    std::string cSourceStore::getText(annotation_t const& _t) const {
        std::string ret;

        auto it = mSources.find(_t.mSourceTextId);

        if (it != mSources.end()) {
            auto& text_desc = it->second;
            auto b          = &text_desc.mText[_t.mStartIndex];
            auto e          = &text_desc.mText[_t.mEndIndex];
            ret             = std::string(b, e);
        }

        return ret;
    }

}
