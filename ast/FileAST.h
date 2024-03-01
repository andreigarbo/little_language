#ifndef FILE_AST
#define FILE_AST

#include <GenericAST.h>
#include <string>

class FileAST : public GenericAST {
    std::string path;

    public:
        FileAST(const std::string& path) : path(std::move(path)) {}
        Value *codegen() {return nullptr;}

};

#endif