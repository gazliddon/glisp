#include "reader.h"
#include "demangle.h"

#include "grammar.h"
#include "symtab.h"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

namespace glisp {
    using namespace boost::spirit;

    template <class Result, class Func>
    struct forwarding_visitor : boost::static_visitor<Result> {
        Func func;
        forwarding_visitor(const Func& f)
            : func(f) {
        }
        forwarding_visitor(Func&& f)
            : func(std::move(f)) {
        }
        template <class Arg>
        Result operator()(Arg&& arg) const {
            return func(std::forward<Arg>(arg));
        }
    };

    template <class Result, class Func>
    forwarding_visitor<Result, std::decay_t<Func>> make_forwarding_visitor(
        Func&& func) {
        return { std::forward<Func>(func) };
    }

    template <typename T>
    std::string v(T& _val) {
        using namespace std;

        if constexpr (std::is_convertible<T*, x3::position_tagged*>()) {
            std::cout << fmt::format(
                "{} ({} {})", demangle(_val), _val.id_first, _val.id_last)
                      << std::endl;
        }

        if constexpr (std::is_convertible<T*, ast::seq_t*>()) {
            auto it = _val.iterator();

            cout << fmt::format("seq of size {}", it->size()) << endl;
            ;

            while (auto p = it->next()) {
                ast::val& x = *p;
                v(x);
            }
        }

        return "";
    }

    template <typename T>
    std::string v(x3::forward_ast<T>& _val) {
        return v(_val.get());
    }

    void cReader::dump() {
        /* auto f = [](auto & _val) { return v(_val); }; */
        /* std::cout << boost::apply_visitor(f, mAst) << std::endl; */
        /* std::cout << std::endl; */
    }

    struct dummy_pos {
        template <typename T, typename Iterator, typename Context>
        inline void on_success(Iterator const& first,
            Iterator const& last,
            T& ast,
            Context const& context) {
            std::cout << "ANNOTATING" << std::endl;
        }
    };

    cReader::reader_reslult_t cReader::read(std::string const& _str) {
        reader_reslult_t ret(_str);

        ret.mText = _str;

        using namespace std;

        auto iter = _str.begin(), end = _str.end();

        using x3::with;
        dummy_pos pos;

        auto const parser = //
            with<grammar::position_cache_tag>(ref(pos))[with<ast::cSymTab>(
                ref(mSymTab))[with<x3::error_handler_tag>(
                ref(ret.mErrors))[grammar::program]]];

        bool r = phrase_parse(
            iter, end, parser, x3::ascii::space_type(), ret.mAst);

        if (auto p_ptr = ret.mAst.get<ast::program>()) {
            if (p_ptr->mForms.size() == 1) {
                ret.mAst = p_ptr->mForms[0];
            }
        }

        if (r && iter == end) {

        } else {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
            cout << *iter << "\n";
        }
        return ret;
    }

    

    cReader::cReader(cReaderSymTab & _symTab) : mSymTab(_symTab){
    }

}
