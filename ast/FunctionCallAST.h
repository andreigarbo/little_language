#ifndef FUNCTION_CALL_AST
#define FUNCTION_CALL_AST

#include "../headers/llvm_commons.h"

class FunctionCallAST : public GenericAST {
    std::string callee;
    std::vector<std::unique_ptr<GenericAST>> args;

    public: 
        FunctionCallAST(
            std::string callee,
            std::vector<std::unique_ptr<GenericAST>> args
        ) : callee(std::move(callee)), args(std::move(args)) {}
        llvm::Value* codegen() {}
};

#endif