#include "ccommand.h"


namespace glisp {



        void cCommand::addCommand(char const* _match, func_t _func) {
            auto r            = std::regex(_match);
            auto func_adapted = [_func](auto const&) { _func(); };

            mFuncs.push_back({ r, func_adapted });
        }

        void cCommand::addCommand(char const* _match, func_args_t _func) {
            auto r = std::regex(_match);
            mFuncs.push_back({ r, _func });
        }

        bool cCommand::processCommand(std::string const& _comm) {
            bool called = false;

            for (auto& com : mFuncs) {
                std::smatch sm;

                if (std::regex_match(_comm, sm, com.first)) {

                    std::vector<std::string> matches;

                    for (auto& m : sm) {
                        matches.push_back(m);
                    }

                    com.second(matches);
                    return true;
                }
            }
            return false;
        }
}

