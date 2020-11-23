#include "citerator.h"

#include "seq.h"
namespace ast {

    template <typename COLL>
    std::unique_ptr<iterator_base_t> mkStlIt(COLL const& _coll) {
        assert(false);
    }
    cIterator::cIterator(set const& _vec) : cIterator(mkStlIt(_vec.mForms)) {
    }

    cIterator::cIterator(program const& _vec) : cIterator(mkStlIt(_vec.mForms)) {
    }

    cIterator::cIterator(bindings const & _vec ) : cIterator(mkStlIt(_vec.mBindings)) {
    }

    cIterator::cIterator(std::unique_ptr<iterator_base_t>&& _it)
        : mpIt(std::move(_it)) {
    }

    cIterator::cIterator(vector const& _vec)
        : cIterator(mkStlIt(_vec.mForms)) {
    }

    cIterator::cIterator(sexp const& _vec)
        : cIterator(mkStlIt(_vec.mForms)) {
    }

    cIterator::cIterator() {
        mpIt = std::make_unique<empty_iterator_t>();
    }

    cIterator cIterator::clone() const {
        return cIterator(mpIt->clone());
    }

    cIterator cIterator::rest() const {
        return cIterator(mpIt->rest());
    }

    size_t cIterator::size() const {
        return mpIt->size();
    }

    size_t cIterator::remaining() const {
        return mpIt->remaining();
    }

    boost::optional<val const&> cIterator::first() const {
        return mpIt->first();
    }

    boost::optional<val const&> cIterator::next() const {
        return mpIt->next();
    }
} /*   ast */
