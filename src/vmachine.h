#ifndef VMACHINE_H_07OQEZTL
#define VMACHINE_H_07OQEZTL

#include <vector>

namespace vmachine {

    enum byte_code {
        op_neg, //  negate the top stack entry
        op_add, //  add top two stack entries
        op_sub, //  subtract top two stack entries
        op_mul, //  multiply top two stack entries
        op_div, //  divide top two stack entries
        op_int, //  push constant integer into the stack
    };

    class vmachine {
      public:
        vmachine( unsigned stackSize = 4096 );

        int top() const ;

        void execute( std::vector<int> const &code );

      private:
        std::vector<int> stack;
        std::vector<int>::iterator stack_ptr;
    };

}

#endif /* end of include guard: VMACHINE_H_07OQEZTL */
