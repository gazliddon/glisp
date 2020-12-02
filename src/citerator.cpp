#include "citerator.h"

#include "seq.h"
namespace ast {

    template <typename COLL>
    std::unique_ptr<seq::iterator_base_t> mkStlIt(COLL & _coll) {

        return std::make_unique<seq::stl_iterator<COLL>>(_coll.begin(), _coll.end());
    }


    cIterator::cIterator(cIterator&& _it) : cIterator(std::move(_it.mpIt)) {
        }

    cIterator::cIterator(cIterator const& _it)
            : cIterator(_it.clone()) {
        }

    cIterator::cIterator(set & _vec) : cIterator(mkStlIt(_vec.mForms)) {
    }


    cIterator::cIterator(std::vector<ast::val> & _valVec) :cIterator(mkStlIt(_valVec)){
    }

    cIterator::cIterator(program & _vec) : cIterator(mkStlIt(_vec.mForms)) {
    }

    cIterator::cIterator(bindings & _vec ) : cIterator(mkStlIt(_vec.mBindings)) {
    }

    cIterator::cIterator(std::unique_ptr<seq::iterator_base_t>&& _it)
        : mpIt(std::move(_it)) {
    }

    cIterator::cIterator(vector & _vec)
        : cIterator(mkStlIt(_vec.mForms)) {
    }

    cIterator::cIterator(sexp & _vec)
        : cIterator(mkStlIt(_vec.mForms)) {
    }

    cIterator::cIterator() {
        mpIt = std::make_unique<seq::empty_iterator_t>();
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

    boost::optional<val &> cIterator::first() const {
        return mpIt->first();
    }

    boost::optional<val &> cIterator::next() const {
        return mpIt->next();
    }
} /*   ast */