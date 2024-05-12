#ifndef FOR_IN_AST_H
#define FOR_IN_AST_H

#include "../headers/llvm_commons.h"

class ForInAST : public GenericAST {
    std::string variableName;
    std::unique_ptr<GenericAST> iterable;
    std::vector<std::unique_ptr<GenericAST>> body;

    public:
        ForInAST(
            std::string variableName,
            std::unique_ptr<GenericAST> iterableName,
            std::vector<std::unique_ptr<GenericAST>> body
        ) : variableName(std::move(variableName)), iterable(std::move(iterable)), body(std::move(body)) {}
        llvm::Value* codegen() {}
};

#endif