///////////////////////////////////////////////////////////////////////////////
//#define BOOST_SPIRIT_X3_DEBUG

#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>

#include "grammar.h"

#include "env.h"
#include "eval.h"
#include "native.h"
#include "run.h"

#include "errors.h"

#include "ccommand.h"
#include "compile.h"
#include "except.h"
#include "reader.h"
#include "native.h"

static char const* banner = R"delim(
   _____ _ _
  / ____| (_)
 | |  __| |_ ___ _ __
 | | |_ | | / __| '_ \
 | |__| | | \__ \ |_) |
  \_____|_|_|___/ .__/
                | |
                |_| For all you lisp parsing fun

)delim";

template<typename... A>
ast::val callNative(ast::Evaluator & _e, std::function<ast::val(ast::Evaluator & _e, ast::cIterator &)> _f, A... a) {
    std::vector<ast::val> fff {a...};
    auto it = ast::cIterator(fff);
    return _f(_e, it);
}

namespace glisp {

    class cGlispReplCommands : public cCommand {

    public:
        cGlispReplCommands(ast::Evaluator& _ev)
            : mEval(_ev) {

            using std::cout;
            using std::endl;

            addCommand("^p$",
                [this](std::vector<std::string> const& _matches) {
                    auto text =callNative(mEval, glisp::slurp, std::string("t.gl"));
                    cout << mEval.to_string(text);

                    auto ast =callNative(mEval, glisp::read_fn, *text.get<std::string>());

                    cout << mEval.to_string(ast) << endl;
                });

            addCommand("^v$",
                [this]() { mEval.readAndEval("define *verbose* true)"); });

            addCommand("^s$", [this]() {
                cout << "Enumerating bindinsg" << endl;
                mEval.enumerateBindings(
                    [this](std::string const& _str, ast::val const& _v) {
                        cout << _str << " : " << mEval.to_string(_v) << endl;
                    });

                mEval.getAstContext().getScoper().dump();
            });
        }

    protected:
        ast::Evaluator& mEval;
    };

    using std::cout;
    using std::endl;

    std::string get_input(std::istream& _in = std::cin) {
        std::string ret;
        std::getline(_in, ret);
        return ret;
    }

    void repl() {
        auto& _in  = std::cin;
        auto& _out = std::cout;

        _out << banner << std::endl;

        _out << "Glisp lisp parser\n";
        _out << "Type an expression...or [q or Q] to quit\n\n";

        ast::Evaluator evaluator;

        /* _out << "including prelude.gl" << endl; */
        /* include(evaluator, "prelude.gl"); */

        bool quit = false;

        cCompiler compiler;

        cGlispReplCommands commands(evaluator);

        commands.addCommand("^q$", [&quit]() { quit = true; });

        evaluator.setCurrentNamespace("user");

        while (!quit) {
            try {
                _out << "user => ";

                auto str = get_input(_in);

                auto wasProcessed = commands.processCommand(str);

                if (!wasProcessed) {
                    using namespace glisp;
                    /* auto ast    = evaluator.read(str); */

                    auto read = evaluator.read(str);

                    _out << evaluator.to_string(read.mAst) << endl;

                    /* auto result = evaluator.eval(read); */
                    
                    /* auto str    = evaluator.to_string(result); */
                    /* _out << str << "\n"; */

                    /* evaluator.set("*1", result); */
                }

            } catch (cEvalError e) {
                _out << "Eval Error : " << e.what() << endl;
            } catch (boost::spirit::x3::expectation_failure<char const*>& e) {
                _out << "Parse Error : " << e.what();
            }
        }
    }
}; // namespace glisp

int main(int argc, char* argv[]) {
    using namespace std;
    using namespace ast;
    using namespace glisp;


    if (false) {

    } else {

        if (argc > 1) {
            ifstream t(argv[1]);
            string str(
                (istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

            ast::Evaluator evaluator;

            auto ast = evaluator.readAndEval(str);

            cout << evaluator.to_string(ast) << std::endl;

        } else {
            glisp::repl();
        }
    }

    return 0;
}
