#ifndef FUNCTION_PROTOTYPE_ARGUMENT_INT_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_INT_AST_H

#include "./FunctionPrototypeArgumentAST.h"
#include "./IntAST.h"
#include "../headers/llvm_commons.h"

class FunctionPrototypeArgumentIntAST : public FunctionPrototypeArgumentAST {
    std::string name;
    std::unique_ptr<IntAST> value;

    public:
        FunctionPrototypeArgumentIntAST(
            std::string name,
            std::unique_ptr<IntAST> value
        ) : FunctionPrototypeArgumentAST(name, std::move(value)), name(std::move(name)), value(std::move(value)) {}
        llvm::Value *codegen() {}
};

#endif