#ifndef INT_AST
#define INT_AST

#include <GenericAST.h>
#include "../headers/llvm_commons.h"

class IntAST : public GenericAST {
    int value;

    public:
        IntAST(int value) : value(value) {}
        Value *codegen() {
            return ConstantInt::get(*TheContext, APInt(32, value, true)); //true means is signed, not sure it will work and maybe only needs 2 params
        }

};  

#endif