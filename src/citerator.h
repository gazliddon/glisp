#ifndef CITERATOR_H_XR1T2AIN
#define CITERATOR_H_XR1T2AIN
#include "ast.h"
#include "seq.h"

namespace ast {

    // Pre-declarations

    class cIterator {

    public:
        cIterator();

        ~cIterator() = default;

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

        boost::optional<val&> first() const;
        boost::optional<val&> next() const;

        void iterate(std::function<void(val&)> _func) const {
            auto it = clone();
            while (auto p = it.next()) {
                _func(*p);
            }
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
        boost::optional<T&> next_of() const {

            while (remaining() != 0) {
                if (auto v = next_as<T>()) {
                    return *v;
                }
            }
            return {};
        }

        template <typename T>
        void iterate(std::function<void(T const&)> _func) const {
            auto it = clone();
            while (auto p = next_of<T>()) {
                _func(*p);
            }
        }

    protected:
        bool mIsConst;
        cIterator(std::unique_ptr<seq::iterator_base_t>&& _it);
        std::unique_ptr<seq::iterator_base_t> mpIt;
    };
}

#endif /* end of include guard: CITERATOR_H_XR1T2AIN */
