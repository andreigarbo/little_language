#ifndef FOR_IN_AST
#define FOR_IN_AST

#include <GenericAST.h>

class ForInAST : public GenericAST {
    unique_ptr<GenericAST> term, array, body;

    public:
        ForInAST(
            unique_ptr<GenericAST> term,
            unique_ptr<GenericAST> array,
            unique_ptr<GenericAST> body
        ) : term(std::move(term)), array(std::move(array)), body(std::move(body)) {}
        Value* codegen() {}
}

#endif