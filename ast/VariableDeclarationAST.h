#ifndef VARIABLE_DECLARATION_AST
#define VARIABLE_DECLARATION_AST

#include "../headers/llvm_commons.h"

class VariableDeclarationAST : public GenericAST {
    std::string type;
    std::string name;
    std::unique_ptr<GenericAST> initialValue;

    public:
        VariableAssignAST(
            std::string type,
            std::string name,
            std::unique_ptr<GenericAST> initialValue = nullptr
        ) : type(std::move(type)), name(std::move(name)), initialValue(std::move(initialValue)) {}
        llvm::Value* codegen() {}
};

#endif