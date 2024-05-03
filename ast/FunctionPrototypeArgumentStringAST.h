#ifndef FUNCTION_PROTOTYPE_ARGUMENT_STRING_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_STRING_AST_H

#include "FunctionPrototypeArgumentAST.h"
#include "StringAST.h"
#include "../headers/llvm_commons.h"

class FunctionPrototypeArgumentStringAST : public FunctionPrototypeArgumentAST {
    std::string name;
    std::unique_ptr<StringAST> value;

    public:
        FunctionPrototypeArgumentStringAST(
            std::string name,
            std::unique_ptr<StringAST> value
        ) : FunctionPrototypeArgumentAST(name, std::move(value)), name(name), value(std::move(value)) {}
        llvm::Value *codegen() {}
};

#endif