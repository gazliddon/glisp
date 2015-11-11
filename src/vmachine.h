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
        vmachine( unsigned stackSize = 4096 )
            : stack( stackSize ), stack_ptr( stack.begin() ) {
        }

        int top() const {
            return stack_ptr[ -1 ];
        };
        void execute( std::vector<int> const &code );

      private:
        std::vector<int> stack;
        std::vector<int>::iterator stack_ptr;
    };

    void vmachine::execute( std::vector<int> const &code ) {
        std::vector<int>::const_iterator pc = code.begin();
        stack_ptr                           = stack.begin();

        while ( pc != code.end() ) {
            switch ( *pc++ ) {
                case op_neg:
                    stack_ptr[ -1 ] = -stack_ptr[ -1 ];
                    break;

                case op_add:
                    --stack_ptr;
                    stack_ptr[ -1 ] += stack_ptr[ 0 ];
                    break;

                case op_sub:
                    --stack_ptr;
                    stack_ptr[ -1 ] -= stack_ptr[ 0 ];
                    break;

                case op_mul:
                    --stack_ptr;
                    stack_ptr[ -1 ] *= stack_ptr[ 0 ];
                    break;

                case op_div:
                    --stack_ptr;
                    stack_ptr[ -1 ] /= stack_ptr[ 0 ];
                    break;

                case op_int:
                    *stack_ptr++ = *pc++;
                    break;
            }
        }
    }
}

#endif /* end of include guard: VMACHINE_H_07OQEZTL */
