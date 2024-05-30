#ifndef BINARY_EXPR_AST
#define BINARY_EXPR_AST

#include "GenericAST.h"
#include <vector>
#include <typeinfo>
#include "LLVMState.h"
#include "StringAST.h"
#include "IntAST.h"
#include "FloatAST.h"
#include "VariableAST.h"
#include "ErrorValue.h"


class BinaryExprAST : public GenericAST {
    char operation;
    std::unique_ptr<GenericAST> leftMember, rightMember;

    public:
        BinaryExprAST(
            char operation,
            std::unique_ptr<GenericAST> leftMember,
            std::unique_ptr<GenericAST> rightMember
        ) : operation(std::move(operation)), leftMember(std::move(leftMember)), rightMember(std::move(rightMember)) {}
        virtual ~BinaryExprAST() = default;
        llvm::Value* codegen() override;
};

#endif