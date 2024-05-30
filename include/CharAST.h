#ifndef CHAR_AST_H
#define CHAR_AST_H

#include "GenericAST.h"
#include "ErrorPrototype.h"
#include <typeinfo>

class CharAST : public GenericAST {
    char value;

    public:
        CharAST(
            char value
        ) : value(value) {}
        virtual ~CharAST() = default;
        llvm::Value *codegen() override;
};

static std::unique_ptr<CharAST> LogErrorPrototypeChar(const char *string){
    LogError(string);
    return nullptr;
}

#endif