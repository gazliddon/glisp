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

    ast::program expand(ast::Evaluator& _ev, ast::program const& _prg) {
        return _prg;
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

        _out << "including prelude.gl" << endl;
        include(evaluator, "prelude.gl");

        bool quit = false;

        while (!quit) {
            try {
                _out << "=> ";

                auto str = get_input(_in);

                if (!str.empty()) {
                    if (str.size() == 1) {
                        switch (str[0]) {
                            case 'q':
                                quit = true;
                                break;
                            case 's':
                                dump(evaluator.mEnv, _out);
                                break;
                            case 'v':
                                evaluator.eval(read("(define *verbose* true)"));
                                break;
                        }
                    } else {
                        auto ast = read(str);
                        /* ast      = expand(evaluator, ast); */
                        auto res = evaluator.eval(ast);

                        glisp::output_string(_out, res);
                        _out << "\n";
                        evaluator.mEnv = evaluator.mEnv.set("*1", res);
                    }
                }
            } catch (boost::spirit::x3::expectation_failure<char const*>& e) {
                _out << "ERROR " << e.what();
            }
        }
    }
}; // namespace glisp
template <typename... Args>
std::vector<ast::val> f(Args... args) {
    std::vector<ast::val> ret(sizeof...(Args));
    ret = { ast::val(args)... };
    return ret;
}

template <typename... Args, std::size_t... Is>
std::tuple<Args...> functo_low(
    std::vector<ast::val> const& arr, std::index_sequence<Is...>) {
    return std::make_tuple(*arr[Is].get_val<Args>()...);
}

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
