#ifndef IMPORT_AST_H
#define IMPORT_AST_H

#include "GenericAST.h"
#include <memory>
#include <string>
#include <llvm/IR/Value.h>

class ImportAST : public GenericAST {
    std::string library;

    public:
        ImportAST(
            std::string library
        ) : library(std::move(library)) {}
        llvm::Value *codegen() override;
};

#endif