#ifndef READER_H_RYMNY8TG
#define READER_H_RYMNY8TG


#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "ast.h"
#include "csymregistry.h"
#include "ctx.h"

namespace glisp {

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

        cReader(ast::cContext & _ctx);
        reader_reslult_t read(std::string const& _str, std::string const & _file = "REPL");

        ast::cContext & mContext;
    };
}

#endif /* end of include guard: READER_H_RYMNY8TG */
