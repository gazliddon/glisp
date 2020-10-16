#include "reader.h"

#include "grammar.h"

namespace glisp {
    ast::program read(std::string const& _str) {
        using namespace std;
        namespace x3 = boost::spirit::x3;

        using x3::ascii::space_type;
        ast::program ast;

        space_type space;
        using x3::with;

        using x3::error_handler_tag;

        using iterator_type = std::string::const_iterator;

        auto iter = _str.begin(), end = _str.end();

        using error_handler_type = x3::error_handler<iterator_type>;

        error_handler_type error_handler(iter, end, std::cerr);

        auto const parser = with<error_handler_tag>(
            std::ref(error_handler))[grammar::program];

        bool r = phrase_parse(iter, end, parser, space, ast);

        if (r && iter == end) {

        } else {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
            cout << *iter << "\n";
        }

        return ast;
    }
}
