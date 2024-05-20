#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include "GenericAST.h"
#include <memory>
#include <string>
#include <llvm/IR/Value.h>

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
        llvm::Value* codegen() override;
};

#endif