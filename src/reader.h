#ifndef READER_H_RYMNY8TG
#define READER_H_RYMNY8TG

// converts string to ast
//
#include "ast.h"

#include <iostream>

namespace glisp {

    class reader_reslult_t {
    public:
        using iterator_type = std::string::const_iterator;
        using position_cache
            = boost::spirit::x3::position_cache<std::vector<iterator_type>>;
        using position_t = boost::iterator_range<iterator_type>;

        reader_reslult_t(
            std::string const& _text, std::string const& _file = "")
            : mFileName(_file)
            , mText(_text)
            , mBegin(mText.cbegin())
            , mEnd(mText.cend())
            , mPositions(position_cache { mBegin, mEnd }) {
        }

        void print_file_line(std::size_t line) const {
        }

        std::string mFileName;
        std::string mText;
        iterator_type mBegin;
        iterator_type mEnd;
        position_cache mPositions;

        ast::program mAst;

        position_t getPos(ast::val const& a) const {
            return mPositions.position_of(a);
        }

        size_t getLine(ast::val const & a) const {
            auto p = getPos(a);
            return getLine(p.begin());
        }

        size_t getLine(iterator_type i) const {
            size_t ret = 0;

            while (i < mEnd) {
                switch (*i) {
                    case '\r':
                    case '\n':
                        ret++;
                        break;
                }
                i++;
            }
            return ret;
        }
    };

    reader_reslult_t read(std::string const& _str);

}

#endif /* end of include guard: READER_H_RYMNY8TG */
