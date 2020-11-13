#ifndef SYMTAB_H_F13BULWR
#define SYMTAB_H_F13BULWR

#include <list>
#include <map>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <boost/optional.hpp>

namespace ast {

    // Regsitry of symbols
    // Means symbols in ast are uint64_t rather than std::string
    // This does no contain symbol bindings

    template <typename K, typename V = std::string>
    class cSymTabBase {
    public:
        virtual ~cSymTabBase() {
        }

        virtual boost::optional<K> registerSymbol(V const& _str) = 0;

        virtual K getIdOrRegister(V const& _str) {
            auto id = getId(_str);

            if (id) {
                return *id;
            } else {
                return *registerSymbol(_str);
            }
        }

        virtual boost::optional<V> const getName(K _id) const = 0;
        virtual boost::optional<K> getId(V const& _str) const = 0;
    };

    template <typename coll>
    boost::optional<typename coll::mapped_type> findMap(
        coll const& _map, typename coll::key_type const& _k) {
        auto it = _map.find(_k);
        if (it == _map.end()) {
            return {};
        } else {
            return it->second;
        }
    }

    class cSymTab : public cSymTabBase<uint64_t> {
    public:
        cSymTab(std::string const& _scope, int64_t _scopeId = -1)
            : mScopeName(_scope) {
        }

        cSymTab()
            : cSymTab("") {
        }

        virtual boost::optional<uint64_t> registerSymbol(
            std::string const& _str) {
            auto it = mSymToId.find(_str);

            if (it != mSymToId.end()) {
                return {};
            } else {
                auto ret = mSyms.size();
                mSyms.push_back(_str);
                mSymToId[_str] = ret;
                return ret;
            }
        }

        virtual boost::optional<uint64_t> getId(std::string const& _str) const {
            return findMap(mSymToId, _str);
        }

        virtual boost::optional<std::string> const getName(uint64_t _id) const {
            if (_id >= mSyms.size()) {
                return {};
            } else {
                return { mSyms[_id] };
            }
        }

        virtual boost::optional<std::string> getScopedName(uint64_t _id) const {
            auto name = getName(_id);
            if (name) {
                return { fmt::format("{}/{}", mScopeName, *name) };
            } else {
                return {};
            }
        };

        std::string const& getScopeName() const {
            return mScopeName;
        }

    protected:
        std::string mScopeName;
        std::unordered_map<std::string, uint64_t> mSymToId;
        std::vector<std::string> mSyms;
    };

    struct scoped_symbol {
        int64_t mScope;
        uint64_t mSymbol;

        bool is_local() const {
            return mScope < 0;
        }
    };

    class cHierachicalSymTab : public cSymTabBase<scoped_symbol> {
    public:
        cHierachicalSymTab(std::string const& _name) {
            mCurrentScopeId = *mScopeToId.registerSymbol(_name);
            mAllSymTabs[mCurrentScopeId] = cSymTab(_name);
        }

        std::pair<uint64_t, int64_t> getLocalSizeIdx() const {
            auto size = int64_t(mLocal.size());
            auto idx  = 0 - (size + 1);
            return { size, idx };
        }

        void pushLocal() {
            auto [size, idx] = getLocalSizeIdx();
            auto name        = fmt::format("LOCAL_{}", size);
            mLocal.push_front({ name, idx });
        }

        void popLocal() {
            mLocal.pop_front();
        }

        virtual boost::optional<scoped_symbol> registerSymbol(
            std::string const& _str) {
            auto id =  mAllSymTabs[mCurrentScopeId].registerSymbol(_str);

            return {{int64_t( mCurrentScopeId ), *id}};
        }

        std::optional<scoped_symbol> registerLocalSymbol(
            std::string const& _str) {
            if (auto id = getId(_str)) {
                return {};
            } else {
            }

            assert(false);
        }

        virtual boost::optional<std::string> const getName(
            scoped_symbol _id) const {

            // is this a local scope?
            if (_id.is_local()) {
                auto idx = 0 - (_id.mScope + 1);
                assert(idx < signed(mLocal.size()));

                auto scope = mLocal.begin();

                for (auto i = 0; i < idx; i++) {
                    scope++;
                }
                return scope->getScopedName(_id.mSymbol);
            }

            if (auto symTab = findMap(mAllSymTabs, _id.mScope)) {
                return symTab->getScopedName(_id.mSymbol);
            }

            return {};
        }

        virtual boost::optional<scoped_symbol> getId(
            std::string const& _str) const {
            auto scopeId = -1;

            // Check locals
            for (auto& s : mLocal) {
                if (auto symId = s.getId(_str)) {
                    return { { scopeId, *symId } };
                }
                scopeId--;
            }

            // Search in scopes
            auto & symTab = mAllSymTabs.find(mCurrentScopeId)->second;

            if (auto symId = symTab.getId(_str)) {
                return { { int64_t( mCurrentScopeId ), *symId } };
            }

            return {};
        }

    protected:
        std::list<cSymTab> mLocal;
        std::map<uint64_t, cSymTab> mAllSymTabs;
        cSymTab mScopeToId;
        uint64_t mCurrentScopeId;
    };

}

#endif /* end of include guard: SYMTAB_H_F13BULWR */
