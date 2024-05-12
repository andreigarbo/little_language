#ifndef IMPORT_AST_H
#define IMPORT_AST_H

#include "../headers/llvm_commons.h"

class ImportAST : public GenericAST {
    std::string library;

    public:
        ImportAST(std::string library) : library(std::move(library)) {}
        llvm::Value *codegen() {}

};  

#endif