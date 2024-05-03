#ifndef FLOAT_AST
#define FLOAT_AST

#include "../headers/llvm_commons.h"

class FloatAST : public GenericAST {
    float value;

    public:
        FloatAST(float value) : value(value) {}
        llvm::Value *codegen() {
            return llvm::ConstantFP::get(*TheContext, llvm::APFloat(value));
        }

};

#endif