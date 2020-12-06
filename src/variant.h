#ifndef VARIANT_H_TAH2LRID
#define VARIANT_H_TAH2LRID

#include <boost/mp11/mpl.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <immer/map.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

namespace ast {
    namespace x3 = boost::spirit::x3;
    using namespace boost::mp11;

    struct Evaluator;
    class cIterator;

    template <typename B, typename... A>
    struct is_base_of_array {
        static constexpr std::array<bool, sizeof...(A)> mBools {
            std::is_base_of<B, A>()...
        };
    };

    template <typename T>
    using add_forward_ast = x3::forward_ast<T>;
    template <typename T>
    struct remove_forward : mp_identity<T> { };

    template <typename T>
    struct remove_forward<x3::forward_ast<T>> : mp_identity<T> { };

    template <typename... A>
    struct variant_base_t : x3::variant<A...> {

        using type      = variant_base_t<A...>;
        using raw_types = mp_list<A...>;

        using base_type_t = x3::variant<A...>;

        static std::array<std::string, sizeof...(A)> mTypeNames;

        using typename base_type_t::base_type;
        using base_type_t::operator=;

        typename base_type::variant_type const& getVar() const {
            return base_type::var;
        }

        typename base_type::variant_type& getVar() {
            return base_type::var;
        }

        variant_base_t() {
        }

        template <typename T>
        variant_base_t(T&& rhs)
            : base_type_t(std::forward<T>(rhs)) {
        }

        variant_base_t(variant_base_t const& rhs)
            : base_type_t(rhs.var) {
        }

        bool is_seq() const {
            assert(false);
        }

        template <typename T>
        bool is() const {
            auto id = base_type_t::var.which();
            return id == mp_find<typename base_type_t::types, T>();
        }
        std::string get_name() const {
            return mTypeNames[base_type::var.which()];
        }

        template <typename T>
        constexpr bool is_forward_ast() {
            return std::is_base_of<x3::forward_ast<T>, T>::value;
        }

        template <typename T>
        T* get_ptr() {
            using namespace std;
            using fwd = x3::forward_ast<T>;
            if (is<T>()) {
                if constexpr (mp_contains<raw_types, fwd>::value) {
                    return boost::get<fwd>(this)->get_pointer();
                } else {
                    return boost::get<T>(this);
                }
            }
            return nullptr;
        }

        template <typename T>
        T const* get_ptr() const {
            using namespace std;
            using fwd = x3::forward_ast<T>;
            if (is<T>()) {

                if constexpr (mp_contains<raw_types, fwd>::value) {
                    return boost::get<fwd>(this)->get_pointer();
                } else {
                    return boost::get<T>(this);
                }
            }
            return nullptr;
        }

        template <typename T>
        boost::optional<T&> get() {
            auto ret = get_ptr<T>();
            if (ret) {
                return { *ret };
            } else
                return {};
        }
        template <typename T>
        boost::optional<T const&> get() const {
            auto ret = get_ptr<T>();
            if (ret) {
                return { *ret };
            } else
                return {};
        }

    protected:
    };

    template <typename... A>
    std::array<std::string, sizeof...(A)> variant_base_t<A...>::mTypeNames
        = { demangle<A>()... };

}

#endif /* end of include guard: VARIANT_H_TAH2LRID */
