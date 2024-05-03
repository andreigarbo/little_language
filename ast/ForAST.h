#ifndef FOR_AST
#define FOR_AST

#include "../headers/llvm_commons.h"

class ForAST : public GenericAST {
    std::unique_ptr<GenericAST> intervalStart, intervalEnd, iterator, body;

    public:
        ForAST(
            std::unique_ptr<GenericAST> intervalStart,
            std::unique_ptr<GenericAST> intervalEnd,
            std::unique_ptr<GenericAST> iterator,
            std::unique_ptr<GenericAST> body
        ) : intervalStart(std::move(intervalStart)), intervalEnd(std::move(intervalEnd)), iterator(std::move(iterator)), body(std::move(body)) {}
        llvm::Value *codegen() {}
};

#endif