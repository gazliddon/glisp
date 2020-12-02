#ifndef SEQ_H_OAAPT8MJ
#define SEQ_H_OAAPT8MJ

#include <algorithm>
#include <boost/optional.hpp>
#include <list>
#include <stddef.h>
#include <vector>

// TBD: Rafactor
// change raw ptrs to std::optional<ast::val&>

// TBD: Write iterator
// I need some way to write using an iterator

// TBD: ConstIterator
// I need an iterator that's const
//
// TBD : Research
// Are there iterator traits I can conform to help me use STL

// NOtes
/*
    Can I represent items with imutable vectors + type interace

    so
        [ immer::vector * ] [ protocols -> [protocol 1, 2, 3 ... n] ]
    128 bit ptr
 */
namespace ast {
    struct val;
}

namespace seq {

    using val_cref_opt = boost::optional<ast::val const&>;
    using val_ref_opt  = boost::optional<ast::val &>;

    struct iterator_base_t {

        // iterate
        // Peeks the value at the iterator
        virtual val_ref_opt first() const = 0;
        // Takes the next item in the sequence
        virtual val_ref_opt next() = 0;

        // creation
        // returns a new iterator dropping without the first item
        virtual std::unique_ptr<iterator_base_t> rest() const;

        // Clone's current sequence
        virtual std::unique_ptr<iterator_base_t> clone() const = 0;

        // Query
        virtual size_t size() const = 0;
        virtual size_t remaining() const = 0;
        virtual bool at_end() const;

        virtual ~iterator_base_t() = default;
        virtual void drop();
        template <typename T>
        void into(T& col) {
            assert(false);
        }
    };

    struct empty_iterator_t : public iterator_base_t {
        virtual val_ref_opt first() const {
            return {};
        }

        virtual val_ref_opt next(){
            return {};
        }

        // creation
        virtual std::unique_ptr<iterator_base_t> rest() const {
            return clone();
        }

        virtual std::unique_ptr<iterator_base_t> clone() const {
            return std::make_unique<empty_iterator_t>();
        }

        // Query
        virtual size_t size() const {
            return 0;
        }

        virtual size_t remaining() const {
            return 0;
        }

    };

    template <typename coll>
    struct stl_iterator : public iterator_base_t {

        using it   = typename coll::iterator;
        using c_it = typename coll::iterator;

        using this_t = stl_iterator<coll>;

        it mIt;
        it mBegin;
        it mEnd;

        stl_iterator(it _begin, it _end, it _it)
            : mIt(_it)
            , mBegin(_begin)
            , mEnd(_end) {
        }

        stl_iterator(it _begin, it _end)
            : stl_iterator(_begin, _end, _begin) {
        }

        stl_iterator(coll const& _vec)
            : stl_iterator(_vec.begin(), _vec.end()) {
        }

        virtual size_t size() const {
            return mEnd - mIt;
        }

        virtual val_ref_opt first() const {
            if (at_end()) {
                return {};
            } else {
                return { *mIt };
            }
        }

        virtual size_t remaining() const {
            return mEnd - mIt;
        }

        virtual val_ref_opt next() {
            if (mIt == mEnd) {
                return {};
            } else {
                auto& ret = *mIt;
                mIt++;
                return { ret };
            }
        }

        virtual std::unique_ptr<iterator_base_t> clone() const {
            auto ret = std::make_unique<this_t>(mIt, mEnd);
            return ret;
        }
    };
}

#endif /* end of include guard: SEQ_H_OAAPT8MJ */
