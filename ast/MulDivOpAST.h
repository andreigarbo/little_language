#ifndef MUL_DIV_OP_AST
#define MUL_DIV_OP_AST

#include "../headers/llvm_commons.h"

class MulDivOpAST : public GenericAST {
    std::unique_ptr<GenericAST> left;
    char op;
    std::unique_ptr<GenericAST> right;

    public:
        MulDivOpAST(
            std::unique_ptr<GenericAST> left,
            char op,
            std::unique_ptr<GenericAST> right
        ) : left(std::move(left)), op(op), right(std::move(right)) {}
        llvm::Value* codegen() {}
};

#endif