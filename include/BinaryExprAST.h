#ifndef BINARY_EXPR_AST
#define BINARY_EXPR_AST

#include "GenericAST.h"
#include <memory>
#include <llvm/IR/Value.h>

class BinaryExprAST : public GenericAST {
    char operation;
    std::unique_ptr<GenericAST> leftMember, rightMember;

    public:
        BinaryExprAST(
            char operation,
            std::unique_ptr<GenericAST> leftMember,
            std::unique_ptr<GenericAST> rightMember
        ) : operation(std::move(operation)), leftMember(std::move(leftMember)), rightMember(std::move(rightMember)) {}
        llvm::Value* codegen() override;
};

#endif