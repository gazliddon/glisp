#ifndef NUMBERS_H_P39YCDW0
#define NUMBERS_H_P39YCDW0

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast {
    namespace x3 = boost::spirit::x3;

    struct int_t {
        int mVal;
    };

    struct uint_t {
        unsigned mVal;
    };

    struct double_t {
        double mVal;
    };

    using number_t = x3::variant<int_t, uint_t, double_t>;
}

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/string/detail/string_parse.hpp>

namespace grammar {
    namespace x3 = boost::spirit::x3;

    using x3::alnum;
    using x3::alpha;
    using x3::bool_;
    using x3::char_;
    using x3::double_;
    using x3::hex;
    using x3::int_;
    using x3::lexeme;
    using x3::lit;
    using x3::no_skip;
    using x3::repeat;
    using x3::rule;
    using x3::space;
    using x3::string;
    using x3::uint_;

    struct size_t_class {};
    rule<size_t_class, std::string> const size_t = "size_t";
    auto const size_t_def = ( lit("8") | lit("16") | lit("32") | lit("64") );

    struct signed_t_class {};
    rule<signed_t_class, char> const signed_t = "signed_t";
    auto const signed_t_def = ( char_('u') | char_('u'));

    struct int_t_class {};
    rule<int_t_class, ast::int_t> const int_t = "int_t";
    auto const int_t_def = 'i' >>  int_;
    BOOST_SPIRIT_DEFINE(int_t);

    struct uint_t_class {};
    rule<uint_t_class, ast::int_t> const uint_t = "uint_t";
    auto const uint_t_def = 'u' >> int_;
    BOOST_SPIRIT_DEFINE(uint_t);
}

#endif /* end of include guard: NUMBERS_H_P39YCDW0 */
