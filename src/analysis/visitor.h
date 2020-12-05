#ifndef VISITOR_H_EXNJWK9O
#define VISITOR_H_EXNJWK9O

#include "../ast.h"
#include "../citerator.h"
#include "../ctx.h"

namespace analysis {
    struct anaylyzer_base_t : boost::static_visitor<void> {
        ast::cContext& mContext;

        anaylyzer_base_t(ast::cContext& _context)
            : mContext(_context) {
        }

        void analyze(ast::val& _val) {
            (*this)(_val);
        }
        template <typename T>
        void analyze(T& _val) {
            (*this)(_val);
        }

        template <typename T>
        void analyzeSeq(T& _val) {
            auto it = ast::cIterator(_val);
            analyzeSeq(it);
        }

        void analyzeSeq(ast::cIterator& _it) {
            (*this)(_it);
        }

        virtual void operator()(double _val) {
        }

        virtual void operator()( float  _val) {

        }
        virtual void operator() (int8_t _val) {

        }
        virtual void operator() (uint8_t _val) {

        }
        virtual void operator() (int16_t _val) {

        }
        virtual void operator() (uint16_t _val) {
        }

        virtual void operator() (int32_t _val) {
        }

        virtual void operator() (uint32_t _val) {
        }

        virtual void operator() (int64_t _val) {
        }

        virtual void operator() (uint64_t _val) {
        }

        virtual void operator()(ast::symbol_t& _sym) {
        }

        virtual void operator()(ast::keyword& _keyword) {
        }

        virtual void operator()(ast::set& _set) {
        }

        virtual void operator()(ast::vector& _val) {
            analyzeSeq(_val);
        }

        virtual void operator()(ast::map& _map) {
        }

        virtual void operator()(ast::meta& _val) {
        }

        virtual void operator()(ast::map_entry& _map_entry) {
        }

        virtual void operator()(ast::sexp& _val) {
            analyzeSeq(_val);
        }

        virtual void operator()(ast::program& _val) {
            analyzeSeq(_val);
        }

        virtual void operator()(ast::let& _val) {
        }

        virtual void operator()(ast::define& _val) {
        }

        virtual void operator()(ast::lambda& _val) {
        }

        virtual void operator()(ast::typed_number_t& _val) {
            boost::apply_visitor(*this, _val.mVal);
        }

        void operator()(ast::cIterator& _it) {
            while (auto p = _it.next()) {
                analyze(*p);
            }
        }

        void operator()(ast::cIterator&& _it) {
            while (auto p = _it.next()) {
                analyze(*p);
            }
        }

        template <typename X>
        void operator()(boost::spirit::x3::forward_ast<X>& _val) {
            return operator()(_val.get());
        }

        template <typename X>
        void operator()(X& _val) {
            fmt::print("Uknown {}", demangle(_val));
        }

        void operator()(ast::val& _val) {
            mAstStack.push(&_val);
            try {
                boost::apply_visitor(*this, _val);
            } catch (char const * _e) {
                fmt::print("Error: {}\n", _e);
            };

            mAstStack.pop();
        }

        std::stack<ast::val*> mAstStack;
    };
}

#endif /* end of include guard: VISITOR_H_EXNJWK9O */
