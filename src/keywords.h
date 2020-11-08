#ifndef KEYWORDS_H_0FQTHEOJ
#define KEYWORDS_H_0FQTHEOJ
// keyword store etc

#include "ast.h"
#include <unordered_map>

namespace ast {
    using keyword_id_t = uint64_t;

    class cKeyWordStore {
        public:
            
            keyword_id_t add(std::string const & _kw) ;
            std::string const & get(keyword_id_t _i) const ;

        protected:
            std::unordered_map<std::string, keyword_id_t> mNameToId;
            std::vector<std::string> mKeywords;
    };

}

#endif /* end of include guard: KEYWORDS_H_0FQTHEOJ */
