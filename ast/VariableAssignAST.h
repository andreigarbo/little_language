#ifndef VARIABLE_ASSIGN_AST
#define VARIABLE_ASSIGN_AST

#include <GenericAST.h>


class VariableAssignAST : public GenericAST {
    unique_ptr<GenericAST> left, right;

    public:
        VariableAssignAST(
            unique_ptr<GenericAST> left,
            unique_ptr<GenericAST> right
        ) : left(std::move(left)), right(std::move(right))
        Value* codegen() {}
}

#endif