#ifndef FUNCTION_PROTOTYPE_ARGUMENT_INT_FLOAT_H
#define FUNCTION_PROTOTYPE_ARGUMENT_INT_FLOAT_H

#include "./FunctionPrototypeArgumentAST.h"
#include "./FloatAST.h"
#include "../headers/llvm_commons.h"

class FunctionPrototypeArgumentFloatAST : public FunctionPrototypeArgumentAST {
    std::string name;
    std::unique_ptr<FloatAST> value;

    public:
        FunctionPrototypeArgumentFloatAST(
            std::string name,
            std::unique_ptr<FloatAST> value
        ) : FunctionPrototypeArgumentAST(name, std::move(value)), name(std::move(name)), value(std::move(value)) {}
        llvm::Value *codegen() {}
};

#endif