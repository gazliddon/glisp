#include "reader.h"
#include "grammar.h"
#include "analysis/scoping.h"

namespace glisp {
    using cReaderSymTab = ast::cSymRegistry;

    cReader::reader_reslult_t cReader::read(std::string const& _str) {

        using namespace boost::spirit;

        reader_reslult_t ret(_str);

        ret.mText = _str;

        using namespace std;

        auto iter = _str.begin(), end = _str.end();

        using x3::with;

        parse_ctx_t context {
            .mScopes = mScopes,
        };

        auto const parser = //
            with<x3::error_handler_tag>(ref(ret.mErrors))[grammar::program];

        auto const parser2 = with<parse_ctx_t>(ref(context))[parser];

        bool r = phrase_parse(
            iter, end, parser2, x3::ascii::space_type(), ret.mAst);

        if (auto p_ptr = ret.mAst.get<ast::program>()) {
            if (p_ptr->mForms.size() == 1) {
                ret.mAst = p_ptr->mForms[0];
            }
        }

        mScopes = analysis::lexicallyScope(mScopes, ret.mAst);


        if (r && iter == end) {

        } else {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
            cout << *iter << "\n";
        }
        return ret;
    }

    ////////////////////////////////////////////////////////////////////////////////



    cReader::cReader(cScoper& _scopes)
        : mScopes(_scopes) {
    }

    cReader::reader_reslult_t::reader_reslult_t(std::string const& _text)
        : mText(_text)
        , mErrors(mText.begin(), mText.end(), std::cerr) {
    }

    cReader::reader_reslult_t::reader_reslult_t()
        : reader_reslult_t("") {
    }

    std::string cReader::reader_reslult_t::getText(
        x3::position_tagged const& _tag) {
        if (_tag.id_first >= 0 && _tag.id_last >= 0) {
            auto x = mErrors.position_of(_tag);
            std::string ret(x.begin(), x.end());
            return ret;
        } else {
            return "CAN'T FIND TEXT";
        }
    }
}
