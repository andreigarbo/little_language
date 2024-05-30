#ifndef STRING_AST_H
#define STRING_AST_H

#include "GenericAST.h"
#include "ErrorPrototype.h"
#include <typeinfo>

class StringAST : public GenericAST {

    public:
        std::string value;

        StringAST(
            std::string value
        ) : value(value) {}
        virtual ~StringAST() = default;
        llvm::Value *codegen() override;
};

static std::unique_ptr<StringAST> LogErrorPrototypeString(const char *string){
    LogError(string);
    return nullptr;
}

#endif