#ifndef CITERATOR_H_XR1T2AIN
#define CITERATOR_H_XR1T2AIN

#include "ast.h"

namespace ast {

    // Pre-declarations
   struct iterator_base_t;

    class cIterator {

    public:
        cIterator() ;

        cIterator(vector const& _vec);
        cIterator(sexp const& _vec);
        cIterator(set const& _vec);
        cIterator(program const& _vec);
        cIterator(bindings const & );

        cIterator clone() const ;

        cIterator rest() const ;

        size_t size() const ;
        size_t remaining() const ;

        boost::optional<val const &> first() const ;
        boost::optional<val const &> next() const ;

    protected:
        cIterator(std::unique_ptr<iterator_base_t> && _it);
        std::unique_ptr<iterator_base_t> mpIt;
    };
}

#endif /* end of include guard: CITERATOR_H_XR1T2AIN */
