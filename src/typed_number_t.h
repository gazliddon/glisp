#ifndef __NUMBER_T_H
#define __NUMBER_T_H

#include <boost/mp11/mpl.hpp>
#include <boost/optional/optional.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast {
    namespace x3 = boost::spirit::x3;

    using namespace boost::mp11;

    // clang-format off
    using number_types = mp_list<
        double
        , float
        , int8_t
        , uint8_t
        , int16_t
        , uint16_t
        , int32_t
        , uint32_t
        , int64_t
        , uint64_t
    >;
    // clang-format on

    using number_variant_t = mp_rename<number_types, x3::variant>;


    struct typed_number_t {

        number_variant_t mVal;

        friend bool operator==(
            typed_number_t const& _lhs, typed_number_t const& _rhs) {
            assert(false);
        }
    };

    boost::optional<typed_number_t> makeTypedNumber(std::string & _type_str, long long val);
}

#endif
