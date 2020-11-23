#ifndef CITERATOR_H_XR1T2AIN
#define CITERATOR_H_XR1T2AIN

#include "ast.h"
#include "seq.h"

namespace ast {

    class cIterator {

    public:
        cIterator() {
            mpIt = std::make_unique<empty_iterator_t>();
        }

        cIterator(vector const& _vec) : cIterator(_vec.mForms){
        }

        cIterator(sexp const& _vec) : cIterator(_vec.mForms){
        }

        cIterator clone() const {
            return cIterator(mpIt->clone());
        }

        cIterator rest() const {
            return cIterator(mpIt->rest());
        }

        size_t size() const {
            return mpIt->size();
        }

        size_t remaining() const {
            return mpIt->remaining();
        }

        val_cref_opt first() const {
            return mpIt->first();
        }

        val_cref_opt next() const {
            return mpIt->next();
        }

    protected:
        cIterator(std::unique_ptr<iterator_base_t> && _it) : mpIt(std::move(_it)) {
        }

        template<typename T>
            cIterator(T const & coll) {
            mpIt = std::make_unique<stl_iterator<T>>(coll.begin(), coll.end());
            }
        std::unique_ptr<iterator_base_t> mpIt;
    };
}

#endif /* end of include guard: CITERATOR_H_XR1T2AIN */
