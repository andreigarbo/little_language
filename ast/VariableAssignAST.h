#ifndef VARIABLE_ASSIGN_AST
#define VARIABLE_ASSIGN_AST

#include "../headers/llvm_commons.h"

class VariableAssignAST : public GenericAST {
    std::unique_ptr<GenericAST> left, right;

    public:
        VariableAssignAST(
            std::unique_ptr<GenericAST> left,
            std::unique_ptr<GenericAST> right
        ) : left(std::move(left)), right(std::move(right)) {}
        llvm::Value* codegen() {}
};

#endif