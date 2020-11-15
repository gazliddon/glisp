#ifndef SCOPER_H_94POSECO
#define SCOPER_H_94POSECO

#include "symtab.h"
#include <cstdint>
#include <stack>
#include <string>

namespace glisp {

    class cScoper {
    public:
        using sym_t = std::pair<uint64_t, uint64_t>;

        static auto constexpr mksym = [](uint64_t _scope, uint64_t _sym) {
            return std::make_pair(_scope, _sym);
        };

        cScoper(std::string _defaultScope);

        void push(uint64_t _scopeId);

        void pushGenScope(std::string const& _scopeBaseName);
        uint64_t genScope(std::string const& _base = "");

        void push(std::string const& _scopeName);
        uint64_t pop();

        boost::optional<sym_t> registerDefaultSymbol(
            std::string const& _string, bool _allowAlreadyExisting = true);

        boost::optional<sym_t> registerSymbol(
            std::string const& _string, bool _allowAlreadyExisting = true);

        boost::optional<sym_t> resolveSymbol(
            std::string const& _str);

        ast::cSymTab const& getCurrentScope() const;
        ast::cSymTab& getCurrentScope();
        uint64_t getCurrentScopeId() const;
        std::string getCurrentScopeName() const;

        boost::optional<ast::cSymTab const&> getScope(uint64_t _scopeId) const;
        boost::optional<ast::cSymTab&> getScope(uint64_t _scopeId);

        boost::optional<std::string> getSymbolName(
            sym_t _sym) const;

        void dump() const {
            for (auto& s : mAllSymbols) {
                fmt::print("SYMTAB {}\n", s.second.getScopeName());
                s.second.dump();
                fmt::print("\n");
            }
        }

        uint64_t getOrRegisterScope(std::string const& _scopeName);
        ast::cSymTab const & getDefaultScope() const;

    protected:

        boost::optional<sym_t> registerSymbol(uint64_t _scopeId, std::string const & _name, bool _allowAlreadyExisting = true);

        ast::cSymTab& getDefaultScope();
        std::list<uint64_t> mScopeStack;
        uint64_t mTempScopeId;
        ast::cSymTab mScopes;
        std::unordered_map<uint64_t, ast::cSymTab> mAllSymbols;
        uint64_t mDefaultScopeId;
    };
}

#endif /* end of include guard: SCOPER_H_94POSECO */
