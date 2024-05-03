#ifndef WHILE_AST
#define WHILE_AST

#include "../headers/llvm_commons.h"

class WhileAST : public GenericAST {
    std::unique_ptr<GenericAST> condition, body;

    public:
        WhileAST(
            std::unique_ptr<GenericAST> condition,
            std::unique_ptr<GenericAST> body
        ) : condition(std::move(condition)), body(std::move(body)) {}
        llvm::Value *codegen() {}
};

#endif