#ifndef FILE_AST
#define FILE_AST

#include <string>
#include "../headers/llvm_commons.h"

class FileAST : public GenericAST {
    std::string path;

    public:
        FileAST(const std::string& path) : path(std::move(path)) {}
        llvm::Value *codegen() {return nullptr;}

};

#endif