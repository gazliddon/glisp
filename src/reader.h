#ifndef READER_H_RYMNY8TG
#define READER_H_RYMNY8TG

// converts string to ast
//
#include "ast.h"
#include "symtab.h"
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <iostream>
#include <stack>

namespace glisp {

    class cScoper {
    public:
        cScoper(uint64_t _scopeId);
         void push(uint64_t _scopeId);

        uint64_t genScope(std::string const & _base = "");

        uint64_t pop();

        uint64_t currentScope() {
            return mScopeStack.top();
        }

    protected:
        std::stack<uint64_t> mScopeStack;
        uint64_t mTempScopeId;
    };

    struct parse_ctx_t {
        ast::cSymTab& mSyms;
        cScoper& mScopes;
    };

    namespace x3 = boost::spirit::x3;

    class cReaderSymTab : public ast::cSymTab {
    public:
        cReaderSymTab() {
        }

        ast::symbol getSymbolRef(std::string const& _name) {
            auto id = getIdOrRegister(_name.c_str());
            return { id };
        }
    };

    class cReader {
    public:
        using iterator_type  = std::string::const_iterator;
        using position_cache = x3::position_cache<std::vector<iterator_type>>;
        using position_t     = boost::iterator_range<iterator_type>;
        using error_handler_type = x3::error_handler<iterator_type>;

        struct reader_reslult_t {
            reader_reslult_t(std::string const& _text)
                : mText(_text)
                , mErrors(mText.begin(), mText.end(), std::cerr) {
            }

            reader_reslult_t()
                : reader_reslult_t("") {
            }

            ast::val mAst;
            std::string mFileName;
            std::string mText;
            error_handler_type mErrors;

            std::string getText(x3::position_tagged const& _tag) {
                if (_tag.id_first >= 0 && _tag.id_last >= 0) {
                    auto x = mErrors.position_of(_tag);
                    std::string ret(x.begin(), x.end());
                    return ret;
                } else {
                    return "CAN'T FIND TEXT";
                }
            }
        };

        cReader(cReaderSymTab& symTab);

        reader_reslult_t read(std::string const& _str);

        void print_file_line(std::size_t line) const {
        }

        position_t getPos(ast::val const& a) const {
            assert(false);
        }

        size_t getLine(ast::val const& a) const {
            auto p = getPos(a);
            return getLine(p.begin());
        }

        void dump();

        size_t getLine(iterator_type i) const {
            size_t ret = 0;
            /* while (i < mEnd) { */
            /*     switch (*i) { */
            /*         case '\r': */
            /*         case '\n': */
            /*             ret++; */
            /*             break; */
            /*     } */
            /*     i++; */
            /* } */
            return ret;
        }

        std::stack<reader_reslult_t> mResults;

        cReaderSymTab& mSymTab;
    };
}

#endif /* end of include guard: READER_H_RYMNY8TG */
