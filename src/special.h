#ifndef SPECIAL_H_TJ2DXLBO
#define SPECIAL_H_TJ2DXLBO

#include <stdlib.h>

namespace glisp {
    enum eSpecialForm :  uint64_t {
        If = 0,
        Apply,
        And,
        Or,
        Define,
        Comment,
        Do,
        Cons,
        Conj,
    };
}


#endif /* end of include guard: SPECIAL_H_TJ2DXLBO */
