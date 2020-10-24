///////////////////////////////////////////////////////////////////////////////
//#define BOOST_SPIRIT_X3_DEBUG

#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>

#include "grammar.h"
#include "tostring.h"

#include "env.h"
#include "eval.h"
#include "run.h"

#include "errors.h"

#include "compile.h"
#include "except.h"
#include "native.h"
#include "reader.h"
#include "tostring.h"

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

class seq_it {

public:
    using Coll = std::vector<ast::val>;
    using It   = Coll::iterator;

    seq_it(std::vector<ast::val>& v)
        : mBegin(v.begin())
        , mEnd(v.end())
        , mIt(mBegin) {
    }

    bool next() {
        if (mIt == mEnd) {
            return false;
        } else {
            mIt++;
            return true;
        }
    }

    ast::val& operator*() const {
        return *mIt;
    }

    ast::val* operator->() const {
        return &*mIt;
    }

protected:
    It mBegin;
    It mEnd;
    It mIt;
};

namespace glisp {

    glisp::reader_reslult_t expand(
        ast::Evaluator& _ev, glisp::reader_reslult_t r) {
        return r;
    }

    std::string get_input(std::istream& _in = std::cin) {
        std::string ret;
        std::getline(_in, ret);
        return ret;
    }

    using std::cout;
    using std::endl;

    void repl() {
        auto& _in  = std::cin;
        auto& _out = std::cout;

        _out << banner << std::endl;

        _out << "Glisp lisp parser\n";
        _out << "Type an expression...or [q or Q] to quit\n\n";

        ast::Evaluator evaluator;
        add_natives(evaluator);

        /* _out << "including prelude.gl" << endl; */
        /* include(evaluator, "prelude.gl"); */

        bool quit = false;

        cCompiler compiler;

        while (!quit) {
            try {
                _out << "=> ";

                auto str = get_input(_in);

                if (str == "q") {
                    quit = true;
                } else if (str == "s") {
                    /* dump(evaluator.mEnv, _out); */
                } else if (str == "v") {
                    evaluator.eval(read("(define *verbose* true)"));
                } else {
                    using namespace glisp;
                    auto ast = read(str);

                    _out << to_string(ast::val(ast.mAst), true)
                         << std::endl;

                    /* ast  = expand(evaluator, ast); */
                    /* /1* compiler.compile(ast.mAst); *1/ */
                    /* auto res = evaluator.eval(ast); */
                    /* auto str = glisp::to_string(res); */
                    /* _out << str << "\n"; */
                    /* evaluator.set("*1", res); */
                }

            } catch (cEvalError e) {
                _out << "Eval Error : " << e.what() << endl;
            }
            catch (boost::spirit::x3::expectation_failure<char const*>& e) {
                _out << "Parse Error : " << e.what();
            }
        }
    }
}; // namespace glisp

int main(int argc, char* argv[]) {
    using namespace std;
    using namespace ast;

    if (false) {

    } else {

        if (argc > 1) {
            ifstream t(argv[1]);
            string str(
                (istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

            auto ast    = glisp::read(str);
            auto as_str = glisp::to_string(val(ast.mAst));
            cout << as_str << std::endl;

        } else {
            glisp::repl();
        }
    }

    return 0;
}
