#include "cstringbuilder.h"
#include <regex>

namespace utils {
    cStringBuilder::cStringBuilder(unsigned _indent)
        : mIndent(_indent)
        , mString("") {
    }

    std::string cStringBuilder::getIndentString() const {
        auto indent = fmt::format("{:{}}", "", mIndent);
        return indent;
    }

    std::string cStringBuilder::addIndents(std::string _line) const {
            using std::regex_replace;
        auto newLine = regex_replace(
            _line, std::regex("\\\n"), "\n" + getIndentString());
        return newLine;
    }

}
