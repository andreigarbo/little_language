#ifndef RETURN_AST_H
#define RETURN_AST_H

#include "../headers/llvm_commons.h"

class ReturnAST : public GenericAST {
    std::unique_ptr<GenericAST> returnValue;

    public:
        ReturnAST(std::unique_ptr<GenericAST> returnValue) : returnValue(std::move(returnValue)) {}
        llvm::Value *codegen() {}

};  

#endif