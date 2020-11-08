#include "ast.h"
#include "eval.h"

namespace ast {

    define::define(sexp const& exp) {
        assert(false);
        /* assert(exp.mForms.size() == 3); */
        /* auto define_id = exp.mForms[0].get_val<symbol>(); */
        /* assert(define_id); */
        /* assert(define_id->mName == std::string("define")); */
        /* auto sym_id = exp.mForms[1].get_val<symbol>(); */
        /* assert(sym_id); */
        /* mSym = *sym_id; */
        /* mVal = exp.mForms[2]; */
    }

    val native_function::call(Evaluator& _e,
        iterator_base_t & _args) const {
        if (_args.size() < unsigned(mNumOfArgs)) {
            auto x = fmt::format("Incorect aegs for native function "
                                 "\nExpected at least {}, got {}\n{}",
                mNumOfArgs,
                _args.size(), 
                _e.to_string(_args, "\n"));

            throw(glisp::cEvalError(x.c_str()));
        }
        return mFunc(_e, _args);
    }

    val::val() {
        *this = ast::nil();
    }

    bool val::to_bool() const {
        if (is<nil>()) {
            return false;
        }

        if (is<bool>()) {
            return *get<bool>();
        }

        return true;
    }

    bool val::is_atom() const {
        auto i = var.which();
        return i < int(mp_size<atoms>());
    }



    bool operator==(meta const& _lhs, meta const& _rhs) {
        assert(false);
    }
    bool operator==(map const& _lhs, map const& _rhs) {
        assert(false);
    }
    bool operator==(map_entry const& _lhs, map_entry const& _rhs) {
        assert(false);
    }
    bool operator==(vector const& _lhs, vector const& _rhs) {
        assert(false);
    }
    bool operator==(native_function const& _lhs, native_function const& _rhs) {
        assert(false);
    }


}

namespace ast {
        val map::get(ast::val const& _key) const {
            auto& hmap = mHashMap;
            for (auto i = hmap.begin(); i != hmap.end(); i++) {
                auto this_key = i->mKey;
                if (this_key == _key) {
                    return i->mValue;
                }
            }
            return val();
        }


        void map::add(val const& _key, val const& _val) {
            auto m = map_entry { .mKey = _key, .mValue = _val };
            mHashMap.push_back(m);
        }

        std::unique_ptr<iterator_base_t> program::iterator() const {
            return std::make_unique<vector_iterator>(mForms);
        }

        std::unique_ptr<iterator_base_t> vector::iterator() const {
            return std::make_unique<vector_iterator>(mForms);
        }

        vector::vector(std::vector<val> const& _init)
            : mForms(_init) {
        }

        
        std::unique_ptr<iterator_base_t> sexp::iterator() const {
            return std::make_unique<vector_iterator>(mForms);
        }

        sexp::sexp(std::vector<val> const& _init)
            : mForms(_init) {
        }

        void sexp::conj(ast::val const& _val) {
            mForms.push_back(_val);
        }

        bool sexp::is(char const* _symName) {
            return false;
        }
}

