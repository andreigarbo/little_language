#ifndef FOR_AST
#define FOR_AST

#include <GenericAST.h>

class ForAST : public GenericAST {
    unique_ptr<GenericAST> condition, body;

    public:
        ForAST(
            unique_ptr<GenericAST> condition,
            unique_ptr<GenericAST> body
        ) : condition(std::move(condition)) {}
        Value *codegen() {}
}

#endif