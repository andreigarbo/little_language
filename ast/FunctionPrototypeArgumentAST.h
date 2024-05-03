#ifndef FUNCTION_PROTOTYPE_ARGUMENT_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_AST_H

#include "../headers/llvm_commons.h"

class FunctionPrototypeArgumentAST : public GenericAST {
    std::string name;
    std::unique_ptr<GenericAST> value;

    public:
        FunctionPrototypeArgumentAST( 
            std::string name,
            std::unique_ptr<GenericAST> value
        ) :  name(std::move(name)), value(std::move(value)) {}
        llvm::Value *codegen() {}
};

#endif