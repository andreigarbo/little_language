#ifndef ARRAY_AST
#define ARRAY_AST

#include <vector>
#include "../headers/llvm_commons.h"

class ArrayAST : public GenericAST {
    std::vector<std::unique_ptr<GenericAST>> value;

    public:
        ArrayAST(
            std::vector<std::unique_ptr<GenericAST>> value
        ) : value(std::move(value)) {}
        llvm::Value *codegen() {return nullptr;}

};

#endif