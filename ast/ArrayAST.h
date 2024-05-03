#ifndef ARRAY_AST
#define ARRAY_AST

#include <vector>
#include "../headers/llvm_commons.h"

template <typename T>
class ArrayAST : public GenericAST {
    std::vector<T> value;

    public:
        ArrayAST(std::vector<T>& value) : value(std::move(value)) {}
        llvm::Value *codegen() {return nullptr;}

};

#endif