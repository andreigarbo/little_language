#ifndef FUNCTION_AST
#define FUNCTION_AST

#include "./FunctionPrototypeAST.h"
#include "../headers/llvm_commons.h"

class FunctionAST : public GenericAST {
    std::unique_ptr<FunctionPrototypeAST> prototype;
    std::vector<std::unique_ptr<GenericAST>> body;

    public:
        FunctionAST(
            std::unique_ptr<FunctionPrototypeAST> prototype,
            std::vector<std::unique_ptr<GenericAST>> body
        ) : prototype(std::move(prototype)), body(std::move(body)) {}
        llvm::Value* codegen() {}
};

#endif