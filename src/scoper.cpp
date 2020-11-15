#include "scoper.h"

#include <iostream>
#include <spdlog/spdlog.h>

namespace glisp {
    using namespace std;
    cScoper::cScoper(std::string _defaultScope)
        : mTempScopeId(0) {

        auto ret = getOrRegisterScope(_defaultScope);

        mDefaultScopeId = ret;

        push(ret);
    }

    uint64_t cScoper::pop() {
        assert(!mScopeStack.empty());
        auto name = mScopes.getName(mScopeStack.front());
        assert(name);
        std::cout << "POPPING SCOPE " << *name << std::endl;
        mScopeStack.erase(mScopeStack.begin());
        return mScopeStack.front();
    }

    void cScoper::push(uint64_t _scopeId) {
        auto scopeName = mScopes.getName(_scopeId);
        assert(scopeName);
        std::cout << "PUSHING SCOPE " << *scopeName << std::endl;
        mScopeStack.push_front(_scopeId);
    }

    uint64_t cScoper::getOrRegisterScope(std::string const& _scopeName) {
        auto ret = mScopes.getId(_scopeName);

        if (!ret) {

            fmt::print("Registering new scope {}\n", _scopeName);
            auto id = *mScopes.registerSymbol(_scopeName);
            mAllSymbols.insert({ id, ast::cSymTab(_scopeName) });
            fmt::print("Registered {}\n", mAllSymbols[id].getScopeName());
            return id;

        } else {
            return *ret;
        }
    }

    uint64_t cScoper::genScope(std::string const& _base) {
        auto name = fmt::format("{}_{}", _base, mTempScopeId++);
        auto ret  = mScopes.registerSymbol(name);
        assert(ret);
        mAllSymbols[*ret] = ast::cSymTab(name);
        return *ret;
    }

    uint64_t cScoper::getCurrentScopeId() const {
        assert(!mScopeStack.empty());
        return mScopeStack.front();
    }

    ast::cSymTab const& cScoper::getCurrentScope() const {
        assert(!mScopeStack.empty());
        auto scopeId = getCurrentScopeId();
        auto it      = mAllSymbols.find(scopeId);
        assert(it != mAllSymbols.end());
        return it->second;
    }

    ast::cSymTab& cScoper::getCurrentScope() {
        assert(!mScopeStack.empty());
        auto scopeId = getCurrentScopeId();
        auto it      = mAllSymbols.find(scopeId);
        assert(it != mAllSymbols.end());
        return it->second;
    }

    std::string cScoper::getCurrentScopeName() const {
        auto scope = getCurrentScope();
        return scope.getScopeName();
    }

    std::pair<uint64_t, uint64_t> cScoper::registerSymbol(
        std::string const& _string) {

        auto& scope = getCurrentScope();

        auto id = scope.registerSymbol(_string);

        if (!id) {
            fmt::print("Can't register symbol {} to scope {}\n",
                _string,
                scope.getScopeName());
            scope.dump();
        }

        assert(id);

        return { getCurrentScopeId(), *id };
    }

    std::pair<uint64_t, uint64_t> cScoper::registerDefaultSymbol(
        std::string const& _string) {

        auto& def = getDefaultScope();

        auto id = def.registerSymbol(_string);

        if (!id) {

            fmt::print("Can't register {} to the default scope\n", _string);

            fmt::print(
                "{} already exists with id {}\n", _string, *def.getId(_string));

            fmt::print("Default scope id: {}, name: {}\n",
                mDefaultScopeId,
                getDefaultScope().getScopeName());

            getDefaultScope().dump();

            assert(id);
        }

        return { mDefaultScopeId, *id };
    }

    std::optional<std::pair<uint64_t, uint64_t>> cScoper::resolveSymbol(
        std::string const& _str) {

        for (auto scopeId : mScopeStack) {
            auto it = mAllSymbols.find(scopeId);

            if (it != mAllSymbols.end()) {
                if (auto symId = it->second.getId(_str)) {
                    return { { scopeId, *symId } };
                }
            }
        }

        return {};
    }

    ast::cSymTab& cScoper::getScope(uint64_t _scopeId) {
        auto it = mAllSymbols.find(_scopeId);

        if (it == mAllSymbols.end()) {
            fmt::print(
                "Could not find default scope with id {}\n", mDefaultScopeId);
            assert(false);
        }

        return it->second;
    }

    ast::cSymTab const& cScoper::getScope(uint64_t _scopeId) const {
        auto it = mAllSymbols.find(_scopeId);

        if (it == mAllSymbols.end()) {
            fmt::print(
                "Could not find default scope with id {}\n", mDefaultScopeId);
            assert(false);
        }

        return it->second;
    }

    ast::cSymTab& cScoper::getDefaultScope() {
        return getScope(mDefaultScopeId);
    }

    void cScoper::pushGenScope(std::string const& _scopeBaseName) {
        auto id = genScope(_scopeBaseName);
        push(id);
    }

    boost::optional<std::string> cScoper::getSymbolName(
        std::pair<uint64_t, uint64_t> _sym) const {
        auto [scopeId, symId] = _sym;
        auto& scope           = getScope(scopeId);

        if (scopeId) {
            return scope.getName(symId);
        }

        return {};
    }

    void cScoper::push(std::string const& _scopeName) {
        auto id = mScopes.getId(_scopeName);

        if (!id) {
            id = mScopes.registerSymbol(_scopeName);
        }
        push(*id);
    }
}
