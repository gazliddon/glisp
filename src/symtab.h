#ifndef SYMTAB_H_F13BULWR
#define SYMTAB_H_F13BULWR

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

namespace ast {

    // Regsitry of symbols
    // Means symbols in ast are uint64_t rather than std::string
    // This does no contain symbol bindings

    class cSymTab {
    public:
        cSymTab()
            : mScope("NOSCOPEZ") {
        }

        std::optional<uint64_t> registerSymbol(std::string const& _str) {
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

        uint64_t getIdOrRegister(std::string const& _str) {
            auto it = mSymToId.find(_str);

            if (it == mSymToId.end()) {
                return *registerSymbol(_str);
            } else {
                return it->second;
            }
        }

        std::optional<std::string const*> const getString(uint64_t _id) const {
            if (_id >= mSyms.size()) {
                return {};
            } else {
                return { &mSyms[_id] };
            }
        }

    protected:
        std::string mScope;
        std::unordered_map<std::string, uint64_t> mSymToId;
        std::vector<std::string> mSyms;
    };

    class cScopedSymTabs {
        std::pair<uint64_t, uint64_t> getIdOrRegister(
            std::string const& _label, std::string const& _scope) {
            assert(false);
        }
    };

}

#endif /* end of include guard: SYMTAB_H_F13BULWR */
