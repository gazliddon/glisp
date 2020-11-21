#ifndef SEQ_H_OAAPT8MJ
#define SEQ_H_OAAPT8MJ

#include <algorithm>
#include <list>
#include <stddef.h>
#include <vector>
#include <boost/optional.hpp>

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

    using val_cref_opt = boost::optional<val const&>;

    struct iterator_base_t {
        virtual size_t size()      = 0;
        virtual val_cref_opt first() = 0;
        virtual val_cref_opt next()  = 0;
        virtual ~iterator_base_t() { }
        virtual std::unique_ptr<iterator_base_t> clone() const = 0;
        virtual size_t remaining()                             = 0;
        virtual std::unique_ptr<iterator_base_t> rest() const {
            auto ret = clone();
            ret->drop();
            return ret;
        }

        void drop() {
            next();
        }

        virtual bool at_end() {
            return remaining() != 0;
        }

        template<typename T>
            void into(T & col) {
                assert(false);
            }
    };

    template <typename A, typename B>
    struct twin_iterator {
        A mA;
        B mB;

        twin_iterator(A&& a, B&& b)
            : mA(std::move(a))
            , mB(std::move(b)) {
        }

        virtual size_t size() {
            return std::min(mA.size(), mB.size());
        }

        virtual val const* first() {
            assert(false);
        }

        virtual val const* next() {
            if (mA.at_end() || mB.at_end()) {
                return nullptr;
            } else {
                // TBD
            }
            assert(false);
        }

        virtual std::unique_ptr<iterator_base_t> rest() const {
            assert(false);
        }

        virtual std::unique_ptr<iterator_base_t> clone() const {
            assert(false);
        }
    };

    template <typename coll>
    struct stl_iterator : public iterator_base_t {

        using it   = typename coll::const_iterator;
        using c_it = typename coll::const_iterator;

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
            : stl_iterator(_vec.cbegin(), _vec.cend()) {
        }

        virtual size_t size() {
            return mEnd - mBegin;
        }

        virtual val_cref_opt first() {
            if (size() > 0) {
                return { *mBegin };
            } else {
                return {};
            }
        }

        virtual size_t remaining() {
            return mEnd - mIt;
        }

        virtual val_cref_opt next() {
            if (mIt == mEnd) {
                return {};
            } else {
                auto & ret = *mIt;
                mIt++;
                return { ret };
            }
        }

        virtual std::unique_ptr<iterator_base_t> clone() const {
            auto ret = std::make_unique<this_t>(mBegin, mEnd);
            return ret;
        }
    };

    using vector_iterator = stl_iterator<std::vector<val>>;
    using list_iterator   = stl_iterator<std::list<val>>;

    struct seq_t {
        virtual std::unique_ptr<iterator_base_t> iterator() const = 0;
        virtual ~seq_t()                                          = default;
    };

    template <typename T>
    using is_seq_t = std::is_base_of<seq_t, T>;
}

#endif /* end of include guard: SEQ_H_OAAPT8MJ */
