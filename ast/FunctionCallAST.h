#ifndef FUNCTION_CALL_AST
#define FUNCTION_CALL_AST

#include <GenericAST.h>

class FunctionCallAST : public GenericAST {
    std::string callee;
    std::vector<unique_ptr<GenericAST>> args;

    public: 
        FunctionCallAST(
            std::string callee,
            std::vector<unique_ptr<GenericAST>> args
        ) : callee(std::move(callee)), args(std::move(args)) {}
        Value* codegen() {}
}

#endif