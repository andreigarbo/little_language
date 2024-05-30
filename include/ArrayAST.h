#ifndef ARRAY_AST_H
#define ARRAY_AST_H

#include "GenericAST.h"
#include "ErrorPrototype.h"
#include <vector>
#include <typeinfo>

class ArrayAST : public GenericAST {
    std::vector<std::unique_ptr<GenericAST>> value;

public:
    ArrayAST(
        std::vector<std::unique_ptr<GenericAST>> value
    ) : value(std::move(value)) {}
    virtual ~ArrayAST() = default;
    llvm::Value *codegen() override;
};

static std::unique_ptr<ArrayAST> LogErrorPrototypeArray(const char *string){
    LogError(string);
    return nullptr;
}

#endif