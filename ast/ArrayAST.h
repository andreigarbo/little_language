#ifndef ARRAY_AST
#define ARRAY_AST

#include <GenericAST.h>
#include <vector>

template <typename T>
class ArrayAST : public GenericAST {
    std::vector<T> value;

    public:
        ArrayAST(std::vector<T>& value) : value(std::move(value)) {}
        Value *codegen() {return nullptr;}

};

#endif