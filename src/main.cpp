///////////////////////////////////////////////////////////////////////////////
//#define BOOST_SPIRIT_X3_DEBUG

#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>

#include "grammar.h"
#include "printer.h"

#include "env.h"
#include "eval.h"
#include "run.h"

#include "errors.h"

#include "native.h"
#include "reader.h"

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

        _out << "Glisp lisp parser\n";
        _out << "Type an expression...or [q or Q] to quit\n\n";

        ast::Evaluator evaluator;
        add_natives(evaluator);

        _out << "including prelude.gl" << endl;
        include(evaluator, "prelude.gl");

        while (true) {
            try {
                _out << "> ";
                auto str = get_input(_in);

                if (str[0] == 'q' || str[0] == 'Q') {
                    break;
                } else if (str[0] == 's') {
                    dump(evaluator.mEnv, _out);
                } else {
                    auto ast = read(str);
                    auto res = evaluator.eval(ast);

                    _out << "-> ";

                    ast::print(res, _out);

                    evaluator.mEnv = evaluator.mEnv.set("*1", res);
                }
            } catch (boost::spirit::x3::expectation_failure<char const*>& e) {
                _out << "ERROR " << e.what();
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

            auto ast = glisp::read(str);
            ast::print(ast, cout);

        } else {
            glisp::repl();
        }
    }

    return 0;
}
