#include "env.h"

namespace env {

    using std::pair;

    using ast::fn;
    using ast::form;
    using ast::atom;
    using ast::form_list;

    static fn const plusFn{2, []( size_t _nargs, form_list const &_flist ) {
                               assert( _nargs == 2 );
                               assert( false );
                           }};

    static fn const minusFn{2, []( size_t _nargs, form_list const &_flist ) {
                                assert( _nargs == 2 );
                                assert( false );
                            }};

    static fn const divFn{2, []( size_t _nargs, form_list const &_flist ) {
                              assert( _nargs == 2 );
                              assert( false );
                          }};

    static fn const mulFn{2, []( size_t _nargs, form_list const &_flist ) {
                              assert( _nargs == 2 );
                              assert( false );
                          }};

    template <typename A>
    pair<string, form> atomToForm( std::string const &_name, A const &_atom ) {
        return {_name, form( atom( _atom ) )};
    }

    env_t initialEnv = {
        atomToForm( "+", plusFn ), atomToForm( "-", minusFn ),
        atomToForm( "*", mulFn ), atomToForm( "/", divFn ),
    };
};
