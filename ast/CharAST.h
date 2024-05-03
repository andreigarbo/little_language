#ifndef CHAR_AST
#define CHAR_AST

#include "../headers/llvm_commons.h"

class CharAST : public GenericAST {
    char value;

    public:
        CharAST(char value) : value(value) {}
        llvm::Value *codegen() {
            return llvm::ConstantInt::get(*TheContext, llvm::APInt(8, value));
        }
};

#endif