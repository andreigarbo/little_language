#ifndef GENERIC_AST_H
#define GENERIC_AST_H

#include <llvm/IR/Value.h>
#include <typeinfo>

class GenericAST {
    public:
        virtual ~GenericAST() = default;
        virtual llvm::Value *codegen() = 0;
};

#endif