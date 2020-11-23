#ifndef CITERATOR_H_XR1T2AIN
#define CITERATOR_H_XR1T2AIN
#include "seq.h"
#include "ast.h"


namespace ast {

    // Pre-declarations

    class cIterator {

    public:
        cIterator() ;
        ~cIterator() = default;

        cIterator(vector const& _vec);
        cIterator(sexp const& _vec);
        cIterator(set const& _vec);
        cIterator(program const& _vec);
        cIterator(bindings const & );
        cIterator(std::vector<ast::val> const & _valVec);

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
