#ifndef SCOPER_H_94POSECO
#define SCOPER_H_94POSECO

#include "csymregistry.h"
#include "symbol_t.h"
#include <cstdint>
#include <stack>
#include <string>

namespace glisp {

    class cScoper {
    public:
        cScoper(std::string _defaultScope);

        using opt_symbol_t = boost::optional<ast::symbol_t>;
        using opt_string   = boost::optional<std::string>;

        // Scope context functions
        void push(uint64_t _scopeId);
        void pushGenScope(std::string const& _scopeBaseName);
        uint64_t genScope(std::string const& _base = "");
        void push(std::string const& _scopeName);
        uint64_t pop();

        // Scope registration
        uint64_t getOrRegisterScope(std::string const& _scopeName);

        // Symbol registration / queries
        opt_symbol_t registerDefaultSymbol(
            std::string const& _string, bool _allowAlreadyExisting = true);

        opt_symbol_t registerSymbol(
            std::string const& _string, bool _allowAlreadyExisting = true);

        opt_symbol_t resolveSymbol(std::string const& _str);

        opt_string getSymbolName(ast::symbol_t _sym) const;

        // Scope queries
        boost::optional<ast::cSymRegistry&> getScopeObject(uint64_t _scopeId);
        boost::optional<ast::cSymRegistry const&> getScope(
            uint64_t _scopeId) const;
        boost::optional<uint64_t> getScopeId(std::string const& _name) const;

        // Current scope queries
        uint64_t getCurrentScopeId() const;
        std::string getCurrentScopeName() const;
        ast::cSymRegistry const& getCurrentScope() const;
        ast::cSymRegistry& getCurrentScope();

        // Default scope queries
        ast::cSymRegistry const& getDefaultScope() const;

        // Debug
        void dump() const {
            for (auto& s : mAllSymbols) {
                fmt::print("SYMTAB {}\n", s.second.getScopeName());
                s.second.dump();
                fmt::print("\n");
            }
        }

    protected:
        opt_symbol_t registerSymbol(uint64_t _scopeId,
            std::string const& _name,
            bool _allowAlreadyExisting = true);

        ast::cSymRegistry& getDefaultScope();
        std::list<uint64_t> mScopeStack;
        uint64_t mTempScopeId;
        ast::cSymRegistry mScopes;
        std::unordered_map<uint64_t, ast::cSymRegistry> mAllSymbols;
        uint64_t mDefaultScopeId;
    };
}

#endif /* end of include guard: SCOPER_H_94POSECO */
