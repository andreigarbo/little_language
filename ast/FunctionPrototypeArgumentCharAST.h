#ifndef FUNCTION_PROTOTYPE_ARGUMENT_CHAR_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_CHAR_AST_H

#include "./FunctionPrototypeArgumentAST.h"
#include "./FloatAST.h"
#include "../headers/llvm_commons.h"

class FunctionPrototypeArgumentCharAST : public FunctionPrototypeArgumentAST {
    std::string name;
    std::unique_ptr<CharAST> value;

    public:
        FunctionPrototypeArgumentCharAST(
            std::string name,
            std::unique_ptr<CharAST> value
        ) : FunctionPrototypeArgumentAST(name, std::move(value)), name(std::move(name)), value(std::move(value)) {}
        llvm::Value *codegen() {}
};

#endif