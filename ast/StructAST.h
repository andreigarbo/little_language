#ifndef STRUCT_AST
#define STRUCT_AST

#include <string>
#include <vector>
#include <any>
#include "../headers/llvm_commons.h"

struct StructField{
    std::string name;
    std::any value;
};

class StructAST : public GenericAST {
    std::string name;
    std::vector<StructField> value;

    public:
        StructAST(const std::string& name, const std::vector<StructField> value) : name(std::move(name)), value(std::move(value)) {}
        llvm::Value *codegen() {return nullptr;}
};

#endif