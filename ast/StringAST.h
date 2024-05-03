#ifndef STRING_AST
#define STRING_AST

#include <string>
#include "../headers/llvm_commons.h"

class StringAST : public GenericAST {
    std::string value;

    public:
        StringAST(std::string& value) : value(std::move(value)) {}
        llvm::Value *codegen() {
            return llvm::ConstantDataArray::getString(*TheContext, value); 
        }
};

#endif