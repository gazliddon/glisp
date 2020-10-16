#ifndef READER_H_RYMNY8TG
#define READER_H_RYMNY8TG

// converts string to ast
//
#include "ast.h"

namespace glisp {
    ast::program read(std::string const& _str);
}

#endif /* end of include guard: READER_H_RYMNY8TG */
