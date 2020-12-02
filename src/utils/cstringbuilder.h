#ifndef CSTRINGBUILDER_H_4ZLWKBNT
#define CSTRINGBUILDER_H_4ZLWKBNT

#include <spdlog/spdlog.h>

namespace utils {

    class cStringBuilder {

    public:
        cStringBuilder(unsigned _indent = 0);

        template <typename... Args>
        void print(fmt::string_view format_str, const Args&... args) {
            auto line = fmt::format(format_str, args...);
            mString += addIndents(line);
        }

    protected:
        unsigned mIndent;
        std::string mString;

        std::string getIndentString() const;
        std::string addIndents(std::string _line) const;
    };

}

#endif /* end of include guard: CSTRINGBUILDER_H_4ZLWKBNT */
