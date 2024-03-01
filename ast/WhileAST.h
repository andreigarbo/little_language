#ifndef WHILE_AST
#define WHILE_AST

#include <GenericAST.h>

class WhileAST : public GenericAST {
    unique_ptr<GenericAST> condition, body;

    public:
        WhileAST(
            unique_ptr<GenericAST> condition,
            unique_ptr<GenericAST> body
        ) : condition(std::move(condition)), body(std::move(body)) {}
        Value *codegen() {}
}

#endif