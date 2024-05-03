#ifndef GENERIC_AST
#define GENERIC_AST

#include "../headers/llvm_commons.h"

class GenericAST {
    public:
        virtual ~GenericAST() = default;
        virtual llvm::Value *codegen(){
            return nullptr;
        }
};

#endif