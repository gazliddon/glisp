#ifndef ENV_H_UCLWRIVL
#define ENV_H_UCLWRIVL

#include "ast.h"



// Environment that glip executes in
// sym table and management of scope

// Notes

// new scope only started by lambda?
// (lambda (a b) ())

//     (let [a 1 b 2]
//         (...stuff...))
// 
// syntactic sugar for
//    (((lambda [a b]) (...stuff...)) 1 2)
//
// Clojure syntax muddied by namespaceing?

namespace env {

    using std::map;
    using ast::form;
    using std::string;

    typedef map< string, form> env_t;

    class cEnv {
        public:
            cEnv(cEnv const * _parent = nullptr);

        protected:
            env_t mEnv;
            cEnv const * mpParent;
    };

}

#endif /* end of include guard: ENV_H_UCLWRIVL */
