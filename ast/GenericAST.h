#ifndef GENERIC_AST
#define GENERIC_AST

#include "llvm/IR/Value.h"

class GenericAST {
    public:
        virtual ~GenericAST() = default;
        virtual Value *codegen();
};

#endif