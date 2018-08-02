#include "run.h"
#include<boost/variant.hpp>

struct env_t {
    typedef boost::variant< int, std::string, std::function<void()>> literal_t;
    std::map<ast::symbol, literal_t> mEnv;

};

ast::list eval(ast::list const & _list, env_t const & _env) {
    // atom
    // collection
    // symbol?
    // special
    
    ast::list ret;

    for(auto const & f : _list.mForms) {
    }

    return ret;
}




