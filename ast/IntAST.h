#ifndef INT_AST
#define INT_AST

#include "../headers/llvm_commons.h"

class IntAST : public GenericAST {
    int value;

    public:
        IntAST(int value) : value(value) {}
        llvm::Value *codegen() {
            return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, value, true)); //true means is signed, not sure it will work and maybe only needs 2 params
        }

};  

#endif