#ifndef FUNCTION_AST
#define FUNCTION_AST

#include <GenericAST.h>
#include <FunctionPrototypeAST.h>

class FunctionAST : public GenericAST {
    unique_ptr<FunctionPrototypeAST> prototype;
    unique_ptr<GenericAST> body;

    public:
        FunctionAST(
            unique_ptr<FunctionPrototypeAST> prototype,
            unique_ptr<GenericAST> body
        ) : prototype(std::move(prototype)), body(std::move(body)) {}
        Value* codegen() {}
}

#endif