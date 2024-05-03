#ifndef BINARY_EXP_AST
#define BINARY_EXP_AST

#include "../headers/llvm_commons.h"

class BinaryExprAST : public GenericAST {
    char operation;
    std::unique_ptr<GenericAST> leftMember, rightMember;

    public:
        BinaryExprAST(
            char operation,
            std::unique_ptr<GenericAST> leftMember,
            std::unique_ptr<GenericAST> rightMember
        ) : operation(operation), leftMember(std::move(leftMember)), rightMember(std::move(rightMember)) {}
        llvm::Value* codegen() {}
};

#endif