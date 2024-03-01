#ifndef IF_AST
#define IF_AST

#include <GenericAST.h>

class IfAST : public GenericAST {
    unique_ptr<GenericAST> condition, thenExpr, elseExpr;

    public:
        IfAST(
            unique_ptr<GenericAST> condition,
            unique_ptr<GenericAST> thenExpr,
            unique_ptr<GenericAST> elseExpr
        ) : condition(std::move(condition)), thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}
        Value *codegen() {}
}

#endif