#ifndef FOR_IN_AST
#define FOR_IN_AST

#include "../headers/llvm_commons.h"

class ForInAST : public GenericAST {
    std::unique_ptr<GenericAST> term, iterable, body;

    public:
        ForInAST(
            std::unique_ptr<GenericAST> term,
            std::unique_ptr<GenericAST> iterable,
            std::unique_ptr<GenericAST> body
        ) : term(std::move(term)), iterable(std::move(iterable)), body(std::move(body)) {}
        llvm::Value* codegen() {}
};

#endif