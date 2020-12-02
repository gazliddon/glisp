#ifndef CITERATOR_H_XR1T2AIN
#define CITERATOR_H_XR1T2AIN
#include "ast.h"
#include "seq.h"

namespace ast {

    // Pre-declarations
    //

    /* class cIteratorBase { */
    /* public: */
    /*     ~cIteratorBase() = default; */

    /*     virtual cIterator clone() const  = 0; */
    /*     virtual cIterator rest() const   = 0; */
    /*     virtual size_t size() const      = 0; */
    /*     virtual size_t remaining() const = 0; */

    /*     virtual boost::optional<val&> first() const = 0; */
    /*     virtual boost::optional<val&> next() const  = 0; */

    /* protected: */
    /* }; */

    class cIterator {

    public:
        // Construction
        cIterator();
        cIterator(cIterator&& _it);
        cIterator(cIterator const& _it);

        ~cIterator() = default;

        // Ast construction
        cIterator(vector& _vec);
        cIterator(sexp& _vec);
        cIterator(set& _vec);
        cIterator(program& _vec);
        cIterator(bindings&);
        cIterator(std::vector<ast::val>& _valVec);

        cIterator clone() const;
        cIterator rest() const;
        size_t size() const;
        size_t remaining() const;

        bool finished() const {
            return remaining() == 0;
        }

        boost::optional<val&> first() const;
        boost::optional<val&> next() const;

        void iterate(std::function<void(val&)> _func) const {
            auto it = clone();
            while (auto p = it.next()) {
                _func(*p);
            }
        }

        bool all(std::function<bool(ast::val const&)> _func) {
            bool ret = true;
            iterate([&_func, &ret](ast::val& _val) {
                if (ret) {
                    ret = _func(_val);
                }
            });
            return ret;
        }

        template <typename T>
        bool allType() {
            return all([](ast::val const& _v) { return _v.is<T>(); });
        }

        template <typename T>
        boost::optional<T&> next_as() const {
            if (auto v = next()) {
                return v->get<T>();
            } else {
                return {};
            }
        }

        template <typename T>
        boost::optional<T&> next_if() const {
            if (auto ret = first_as<T>()) {
                drop();
                return ret;
            } else {
                return {};
            }
        }

        template <typename T>
        boost::optional<T&> first_as() const {
            if (auto v = first()) {
                return v->get<T>();
            } else {
                return {};
            }
        }

        template <typename T>
        boost::optional<T&> next_of() const {

            while (remaining() != 0) {
                if (auto v = next_as<T>()) {
                    return *v;
                }
            }
            return {};
        }

        template <typename T>
        void iterate(std::function<void(T&)> _func) const {
            auto it = clone();
            while (auto p = next_of<T>()) {
                _func(*p);
            }
        }

        void drop() const {
            next();
        }

        std::vector<ast::val> into() {
            std::vector<ast::val> _dest;
            _dest.resize(size());
            _dest.resize(0);
            iterate([&_dest](ast::val& _val) {
                _dest.push_back(_val);
            });
            return _dest;
        }

        template <unsigned N>
        std::vector<boost::optional<ast::val&>> into() {
            std::vector<boost::optional<ast::val&>> ret;
            ret.resize(N);
            into_lo(ret, std::make_index_sequence<N> {});
            return ret;
        }
        template <typename... A>
        std::tuple<boost::optional<A&...>> into() {
            auto v = into<sizeof...(A)>();
            return std::make_tuple<boost::optional<A&...>>(next_as<A>()...);
        }


    protected:
        template <typename C, typename T, T... ints>
        void into_lo(C& ret, std::integer_sequence<T, ints...> int_seq) {
            ((ret[ints] = next()), ...);
        }
        cIterator(std::unique_ptr<seq::iterator_base_t>&& _it);
        std::unique_ptr<seq::iterator_base_t> mpIt;
    };
}

#endif /* end of include guard: CITERATOR_H_XR1T2AIN */
