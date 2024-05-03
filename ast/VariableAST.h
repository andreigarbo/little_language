#ifndef VARIABLE_AST
#define VARIABLE_AST

#include <string>
#include "../headers/llvm_commons.h"

class VariableAST : public GenericAST {
    std::string name;
    int type;
    std::unique_ptr<GenericAST> value;


    public:
        VariableAST(
            std::string name,
            int type = 0,
            std::unique_ptr<GenericAST> value = nullptr
        ) : type(type), name(std::move(name)), value(std::move(value)){}
        llvm::Value* codegen() {}
};

#endif