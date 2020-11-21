#ifndef READER_H_RYMNY8TG
#define READER_H_RYMNY8TG


#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "ast.h"
#include "csymregistry.h"
#include "scoper.h"

namespace glisp {

    struct parse_ctx_t {
        cScoper& mScopes;
    };

    namespace x3 = boost::spirit::x3;


    class cReader {
    public:
        using iterator_type  = std::string::const_iterator;
        using error_handler_type = x3::error_handler<iterator_type>;

        struct reader_reslult_t {
            reader_reslult_t(std::string const& _text);
            std::string getText(x3::position_tagged const& _tag) ;
            reader_reslult_t();

            ast::val mAst;
            std::string mFileName;
            std::string mText;
            error_handler_type mErrors;
        };

        cReader(cScoper & scoper);
        reader_reslult_t read(std::string const& _str);

        cScoper& mScopes;
    };
}

#endif /* end of include guard: READER_H_RYMNY8TG */
