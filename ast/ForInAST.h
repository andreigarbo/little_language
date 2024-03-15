#ifndef FOR_IN_AST
#define FOR_IN_AST

#include <GenericAST.h>

class ForInAST : public GenericAST {
    unique_ptr<GenericAST> term, iterable, body;

    public:
        ForInAST(
            unique_ptr<GenericAST> term,
            unique_ptr<GenericAST> iterable,
            unique_ptr<GenericAST> body
        ) : term(std::move(term)), iterable(std::move(iterable)), body(std::move(body)) {}
        Value* codegen() {}
}

#endif