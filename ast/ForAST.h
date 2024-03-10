#ifndef FOR_AST
#define FOR_AST

#include <GenericAST.h>

class ForAST : public GenericAST {
    unique_ptr<GenericAST> intervalStart, intervalEnd, iterator, body;

    public:
        ForAST(
            unique_ptr<GenericAST> intervalStart,
            unique_ptr<GenericAST> intervalEnd,
            unqiue_ptr<GenericAST> iterator,
            unique_ptr<GenericAST> body
        ) : intervalStart(std::move(intervalStart)), intervalEnd(std::move(intervalEnd)), iterator(std::move(iterator)), body(std::move(body)) {}
        Value *codegen() {}
}

#endif