#ifndef STRING_AST
#define STRING_AST

#include <string>
#include <GenericAST.h>

class StringAST : public GenericAST {
    std::string value;

    public:
        StringAST(std::string& value) : value(std::move(value)) {}
        Value *codegen() {return nullptr;}
};

#endif