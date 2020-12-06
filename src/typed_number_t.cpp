#include "typed_number_t.h"

namespace ast {

    boost::optional<typed_number_t> makeTypedNumber(std::string & type_str, long long val) {

        typed_number_t value;

        if (type_str == "u8") {
            value.mVal = static_cast<uint8_t>(val);
            return value;
        }

        if (type_str == "u16") {
            value.mVal = static_cast<uint16_t>(val);
            return value;
        }

        if (type_str == "u32") {
            value.mVal = static_cast<uint32_t>(val);
            return value;
        }

        if (type_str == "u64") {
            value.mVal = static_cast<uint64_t>(val);
            return value;
        }

        if (type_str == "i8") {
            value.mVal = static_cast<int8_t>(val);
            return value;
        }

        if (type_str == "i16") {
            value.mVal = static_cast<int16_t>(val);
            return value;
        }

        if (type_str == "i32") {
            value.mVal = static_cast<int32_t>(val);
            return value;
        }

        if (type_str == "i64") {
            value.mVal = static_cast<int64_t>(val);
            return value;
        }
        return {};
    }
}
