#include "vmachine.h"

namespace vmachine {

    vmachine::vmachine( unsigned stackSize  )
        : stack( stackSize ), stack_ptr( stack.begin() ) {
    }

    int vmachine::top() const {
        return stack_ptr[ -1 ];
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
