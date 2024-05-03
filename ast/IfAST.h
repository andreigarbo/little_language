#ifndef IF_AST
#define IF_AST

#include "../headers/llvm_commons.h"

class IfAST : public GenericAST {
    std::unique_ptr<GenericAST> condition;
    std::vector<std::unique_ptr<GenericAST>> thenExpr, elseExpr;

    public:
        IfAST(
            std::unique_ptr<GenericAST> condition,
            std::vector<std::unique_ptr<GenericAST>> thenExpr,
            std::vector<std::unique_ptr<GenericAST>> elseExpr = {}
        ) : condition(std::move(condition)), thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}
        llvm::Value *codegen() {}
};

#endif