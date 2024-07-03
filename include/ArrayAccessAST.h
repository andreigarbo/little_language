#ifndef ARRAY_ACCESS_AST_H
#define ARRAY_ACCESS_AST_H

#include "GenericAST.h"
#include "ErrorPrototype.h"
#include <vector>
#include <typeinfo>

class ArrayAccessAST : public GenericAST {
    std::string array_name;
    std::unique_ptr<GenericAST> array_index;
    bool store;
    std::unique_ptr<GenericAST> value;
public:
    ArrayAccessAST(
        std::string array_name,
        std::unique_ptr<GenericAST> array_index,
        bool store,
        std::unique_ptr<GenericAST> value
    ) : array_name(std::move(array_name)), array_index(std::move(array_index)), store(store), value(std::move(value)) {}
    virtual ~ArrayAccessAST() = default;
    llvm::Value *codegen() override;
};

static std::unique_ptr<ArrayAccessAST> LogErrorPrototypeArrayAccess(const char *string){
    LogError(string);
    return nullptr;
}

#endif