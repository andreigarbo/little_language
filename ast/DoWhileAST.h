#ifndef DO_WHILE_AST
#define DO_WHILE_AST

#include "../headers/llvm_commons.h"

class DoWhileAST : public GenericAST {
    std::unique_ptr<GenericAST> body, condition;

    public:
        DoWhileAST(
            std::unique_ptr<GenericAST> body,
            std::unique_ptr<GenericAST> condition
        ) : body(std::move(body)), condition(std::move(condition)) {}
        llvm::Value* codegen() {}
};

#endif