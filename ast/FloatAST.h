#ifndef FLOAT_AST
#define FLOAT_AST

#include <GenericAST.h>
#include "llvm_commons.h"

class FloatAST : public GenericAST {
    float value;

    public:
        FloatAST(float value) : value(value) {}
        Value *codegen() {
            return ConstantFP::get(*TheContext, APFloat(value));
        }

};

#endif