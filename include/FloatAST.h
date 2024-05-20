#ifndef FLOAT_AST_H
#define FLOAT_AST_H

#include "GenericAST.h"
#include <memory>
#include <llvm/IR/Value.h>

class FloatAST : public GenericAST {
    float value;

    public:
        FloatAST(
            float value
        ) : value(value) {}
        llvm::Value *codegen() override;
};

#endif