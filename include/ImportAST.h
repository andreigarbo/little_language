#ifndef IMPORT_AST_H
#define IMPORT_AST_H

#include "GenericAST.h"
#include "ErrorPrototype.h"
#include <typeinfo>

class ImportAST : public GenericAST {
    std::string library;

    public:
        ImportAST(
            std::string library
        ) : library(std::move(library)) {}
        virtual ~ImportAST() = default;
        llvm::Value *codegen() override;
};

static std::unique_ptr<ImportAST> LogErrorPrototypeImport(const char *string){
    LogError(string);
    return nullptr;
}

#endif