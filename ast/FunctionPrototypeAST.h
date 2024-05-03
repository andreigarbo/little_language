#ifndef PROTOTYPE_AST
#define PROTOTYPE_AST

#include "../headers/llvm_commons.h"

class FunctionPrototypeAST : public GenericAST {
    std::string returnType;
    std::string name;
    std::vector<std::unique_ptr<GenericAST>> arguments;


    public:
        FunctionPrototypeAST(
            std::string returnType,
            std::string name,
            std::vector<std::unique_ptr<GenericAST>> arguments
        ) : returnType(returnType), name(name), arguments(std::move(arguments)) {}
        llvm::Value *codegen() {}
};

#endif