#ifndef BINARY_EXP_AST
#define BINARY_EXP_AST

#include <GenericAST.h>

class BinaryExprAST : public GenericAST {
    char operation;
    unique_ptr<GenericAST> leftMember, rightMember;

    public:
        BinaryExprAST(
            char operation,
            unique_ptr<GenericAST> leftMember,
            unique_ptr<GenericAST> rightMember
        ) : operation(operation), leftMember(std::move(leftMember)), rightMember(std::move(rightMember)) {}
        Value* codegen() {}
}

#endif