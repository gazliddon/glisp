#ifndef SOURCE_H_HJG0DA2F
#define SOURCE_H_HJG0DA2F

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace ast {
    // keeps all ingested source code
    // in one place
    // Keeps a list of tags into the source that can
    // be retrieved w a source file ID and a start / end position

    using source_key_t = uint32_t;

    struct source_fragment_t {
        std::string mText;
        std::string mFileName;
        int line;
    };

    struct annotation_t {
        source_key_t mSourceTextId;
        uint32_t mStartIndex;
        uint32_t mEndIndex;
    };

    class cSourceStore {

    public:
        cSourceStore();

        source_key_t addSourceText(
            std::string const& _text, std::string const& _fileName = "NO FILE");

        std::string getText(annotation_t const& _t) const;

        void annotate(annotation_t& _slice,
            source_key_t _key,
            uint32_t _start,
            uint32_t _end);

        source_fragment_t getSourceFragment(annotation_t const&) const;

    protected:
        int getLineNumber(annotation_t const* _t) const;

        source_key_t mKey;

        struct source_text_t {
            std::string mText;
            std::string mFileName;
            source_key_t mSourceKey;
        };

        std::unordered_map<source_key_t, source_text_t> mSources;
    };

}

#endif /* end of include guard: SOURCE_H_HJG0DA2F */
