#ifndef ADD_SUB_OP_AST
#define ADD_SUB_OP_AST

#include "../headers/llvm_commons.h"

class AddSubOpAST : public GenericAST {
    std::unique_ptr<GenericAST> left;
    char op;
    std::unique_ptr<GenericAST> right;

    public:
        AddSubOpAST(
            std::unique_ptr<GenericAST> left,
            char op,
            std::unique_ptr<GenericAST> right
        ) : left(std::move(left)), op(op), right(std::move(right)) {}
        llvm::Value* codegen() {}
};

#endif