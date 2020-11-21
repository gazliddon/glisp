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
    class cSymRegistryBase {
    public:
        virtual ~cSymRegistryBase() = default;

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

    class cSymRegistry : public cSymRegistryBase<uint64_t> {
    public:
        cSymRegistry(std::string const& _scope)
            : mScopeName(_scope) {
        }

        cSymRegistry()
            : cSymRegistry("") {
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

        void dump() const { 
            for(auto & p : mSymToId) {
                fmt::print("{} : {}\n", p.second, *getScopedName(p.second));
            }
        }

    protected:
        std::string mScopeName;
        std::unordered_map<std::string, uint64_t> mSymToId;
        std::vector<std::string> mSyms;
    };


}

#endif /* end of include guard: SYMTAB_H_F13BULWR */
