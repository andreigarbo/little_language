#ifndef FLOAT_AST_H
#define FLOAT_AST_H

#include "GenericAST.h"
#include "ErrorPrototype.h"
#include <typeinfo>

class FloatAST : public GenericAST {
    float value;

    public:
        FloatAST(
            float value
        ) : value(value) {}
        virtual ~FloatAST() = default;
        llvm::Value *codegen() override;
};

static std::unique_ptr<FloatAST> LogErrorPrototypeFloat(const char *string){
    LogError(string);
    return nullptr;
}

#endif