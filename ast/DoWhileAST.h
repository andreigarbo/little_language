#ifndef DO_WHILE_AST
#define DO_WHILE_AST

#include <GenericAST.h>

class DoWhileAst : public GenericAST {
    unique_ptr<GenericAST> body, condition;

    public:
        DoWhileAst(
            unique_ptr<GenericAST> body,
            unique_ptr<GenericAST> condition
        ) : body(std::move(body)), condition(std::move(condition)) {}
        Value* codegen() {}
}