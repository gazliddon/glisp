#ifndef SCOPER_H_94POSECO
#define SCOPER_H_94POSECO

#include "symtab.h"
#include <cstdint>
#include <stack>
#include <string>

namespace glisp {

    class cScoper {
    public:
        cScoper(std::string _defaultScope);

        void push(uint64_t _scopeId);

        void pushGenScope(std::string const& _scopeBaseName);
        uint64_t genScope(std::string const& _base = "");

        void push(std::string const& _scopeName);
        uint64_t pop();


        std::pair<uint64_t, uint64_t> registerDefaultSymbol(std::string const & _string);

        std::pair<uint64_t, uint64_t> registerSymbol(std::string const & _string);
        std::optional<std::pair<uint64_t, uint64_t>> resolveSymbol(
            std::string const& _str);

        ast::cSymTab const &  getCurrentScope() const;
        ast::cSymTab &  getCurrentScope() ;
        uint64_t getCurrentScopeId() const;
        std::string getCurrentScopeName() const;

        ast::cSymTab const & getScope(uint64_t _scopeId) const;
        ast::cSymTab& getScope(uint64_t _scopeId) ;

        boost::optional<std::string> getSymbolName(std::pair<uint64_t, uint64_t> _sym) const;

        void dump() const {
            for(auto & s : mAllSymbols) {
                fmt::print("SYMTAB {}\n", s.second.getScopeName());
                s.second.dump();
                fmt::print("\n");
            }
        }


    protected:
        uint64_t getOrRegisterScope(std::string const & _scopeName);

        ast::cSymTab & getDefaultScope() ;
        std::list<uint64_t> mScopeStack;
        uint64_t mTempScopeId;
        ast::cSymTab mScopes;
        std::unordered_map<uint64_t, ast::cSymTab> mAllSymbols;
        uint64_t mDefaultScopeId;
    };
}

#endif /* end of include guard: SCOPER_H_94POSECO */
