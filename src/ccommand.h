#ifndef CCOMMMAND_H_IKJDJXEU
#define CCOMMMAND_H_IKJDJXEU

#include <regex>

namespace glisp {

    class cCommand {
    public:
        cCommand() = default;

        using func_t = std::function<void(void)>;
        using func_args_t
            = std::function<void(std::vector<std::string> const&)>;

        void addCommand(char const* _match, func_t _func);

        void addCommand(char const* _match, func_args_t _func);

        bool processCommand(std::string const& _comm);

    protected:
        std::vector<std::pair<std::regex, func_args_t>> mFuncs;
    };
}

#endif /* end of include guard: CCOMMMAND_H_IKJDJXEU */
