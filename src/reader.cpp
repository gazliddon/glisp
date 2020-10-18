#include "reader.h"

#include "grammar.h"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

namespace glisp {
    using namespace boost::spirit;

    reader_reslult_t read(std::string const& _str) {
        using namespace std;

        reader_reslult_t ret(_str);

        using x3::ascii::space_type;

        ast::program ast;

        space_type space;
        using x3::with;

        using x3::error_handler_tag;

        using iterator_type  = std::string::const_iterator;

        auto iter = _str.begin(), end = _str.end();

        using error_handler_type = x3::error_handler<iterator_type>;

        error_handler_type error_handler(iter, end, std::cerr);

        parse_context ctx;

        auto const parser = with<grammar::position_cache_tag>(std::ref(ret.mPositions))[
            with<parse_context>(std::ref(ctx))[with<error_handler_tag>(
                std::ref(error_handler))[grammar::program]]];

        bool r = phrase_parse(iter, end, parser, space, ret.mAst);

        if (r && iter == end) {

        } else {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
            cout << *iter << "\n";
        }

        return ret;
    }
}
