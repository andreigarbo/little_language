#ifndef INT_AST_H
#define INT_AST_H

#include "GenericAST.h"
#include "ErrorPrototype.h"
#include <typeinfo>

class IntAST : public GenericAST {
    int value;

    public:
        IntAST(
            int value
        ) : value(value) {}
        virtual ~IntAST() = default;
        llvm::Value *codegen() override;
};

static std::unique_ptr<IntAST> LogErrorPrototypeInt(const char *string){
    LogError(string);
    return nullptr;
}

#endif