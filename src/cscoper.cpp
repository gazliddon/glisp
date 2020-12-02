#include "cscoper.h"

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

    boost::optional<uint64_t> cScoper::getScopeId(
        std::string const& _scopeName) const {
        return mScopes.getId(_scopeName);
    }

    uint64_t cScoper::getOrRegisterScope(std::string const& _scopeName) {
        auto ret = getScopeId(_scopeName);

        if (!ret) {
            auto id = *mScopes.registerSymbol(_scopeName);
            mAllSymbols.insert({ id, ast::cSymRegistry(_scopeName) });
            fmt::print(
                "Registered {}:{}\n", mAllSymbols[id].getScopeName(), id);
            return id;

        } else {
            return *ret;
        }
    }

    uint64_t cScoper::genScope(std::string const& _base) {
        auto name = fmt::format("{}_{}", _base, mTempScopeId++);
        auto ret  = mScopes.registerSymbol(name);
        assert(ret);
        mAllSymbols[*ret] = ast::cSymRegistry(name);
        return *ret;
    }

    uint64_t cScoper::getCurrentScopeId() const {
        assert(!mScopeStack.empty());
        return mScopeStack.front();
    }

    ast::cSymRegistry const& cScoper::getCurrentScope() const {
        assert(!mScopeStack.empty());
        auto scopeId = getCurrentScopeId();
        auto it      = mAllSymbols.find(scopeId);
        assert(it != mAllSymbols.end());
        return it->second;
    }

    ast::cSymRegistry& cScoper::getCurrentScope() {
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

    cScoper::opt_symbol_t cScoper::registerSymbol(uint64_t _scopeId,
        std::string const& _name,
        bool _allowAlreadyExisting) {

        if (auto scope = getScopeObject(_scopeId)) {
            if (_allowAlreadyExisting) {
                auto id = scope->getIdOrRegister(_name);
                return ast::symbol_t(_scopeId, id);
            } else {
                if (auto id = scope->registerSymbol(_name)) {
                    return ast::symbol_t(_scopeId, *id);
                }
            }

        } else {
            fmt::print("Can't find scope {} when trying to register {}",
                _scopeId,
                _name);
            assert(false);
        }

        return {};
    }

    cScoper::opt_symbol_t cScoper::registerSymbol(
        std::string const& _string, bool _allowAlreadyExisting) {
        return registerSymbol(
            getCurrentScopeId(), _string, _allowAlreadyExisting);
    }

    cScoper::opt_symbol_t cScoper::registerDefaultSymbol(
        std::string const& _string, bool _allowAlreadyExisting) {

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
        return ast::symbol_t(mDefaultScopeId, *id);
    }

    cScoper::opt_symbol_t cScoper::resolveSymbol(std::string const& _str) {

        /* fmt::print("Searching for {}", _str); */

        for (auto scopeId : mScopeStack) {
            auto it = mAllSymbols.find(scopeId);

            if (it != mAllSymbols.end()) {
                auto& syms = it->second;
                if (auto symId = syms.getId(_str)) {
                    /* fmt::print("!found in {}", syms.getScopeName()); */
                    return { { scopeId, *symId } };
                }
                /* fmt::print("Not found in {}", syms.getScopeName()); */
            }
        }

        return {};
    }

    boost::optional<ast::cSymRegistry&> cScoper::getScopeObject(
        uint64_t _scopeId) {
        auto it = mAllSymbols.find(_scopeId);

        if (it == mAllSymbols.end()) {
            fmt::print(
                "Could not find default scope with id {}\n", mDefaultScopeId);
            assert(false);
        }

        return it->second;
    }

    boost::optional<ast::cSymRegistry const&> cScoper::getScope(
        uint64_t _scopeId) const {
        auto it = mAllSymbols.find(_scopeId);

        if (it != mAllSymbols.end()) {
            return it->second;
        }

        fmt::print("Could not find scope with id {}\n", mDefaultScopeId);
        return {};
    }

    ast::cSymRegistry const& cScoper::getDefaultScope() const {
        auto scope = getScope(mDefaultScopeId);
        assert(scope);
        return *scope;
    }

    ast::cSymRegistry& cScoper::getDefaultScope() {
        auto scope = getScopeObject(mDefaultScopeId);
        assert(scope);
        return *scope;
    }

    void cScoper::pushGenScope(std::string const& _scopeBaseName) {
        auto id = genScope(_scopeBaseName);
        push(id);
    }

    cScoper::opt_string cScoper::getSymbolName(ast::symbol_t _sym) const {

        if (auto scope = getScope(_sym.mScope)) {
            return scope->getName(_sym.mId);
        }

        return {};
    }
    cScoper::opt_string cScoper::getScopedSymbolName(ast::symbol_t _sym) const {

        if (auto scope = getScope(_sym.mScope)) {
            return scope->getScopedName(_sym.mId);
        }

        return {};
    }


    void cScoper::push(std::string const& _scopeName) {
        auto id = getOrRegisterScope(_scopeName);
        push(id);
    }
}
