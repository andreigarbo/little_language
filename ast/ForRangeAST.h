#ifndef FOR_RANGE_AST_H
#define FOR_RANGE_AST_H

#include "../headers/llvm_commons.h"
//auto forRangeAST = std::make_unique<ForRangeAST>(std::move(variableName), std::move(fromValue), std::move(toValue), iterate); 

class ForRangeAST : public GenericAST {
    std::string variableName;
    std::unique_ptr<GenericAST> fromValue, toValue;
    int iterate;
    std::vector<std::unique_ptr<GenericAST>> body;

    public:
        ForRangeAST(
            std::string variableName,
            std::unique_ptr<GenericAST> fromValue,
            std::unique_ptr<GenericAST> toValue,
            int iterate,
            std::vector<std::unique_ptr<GenericAST>> body
        ) : variableName(std::move(variableName)), fromValue(std::move(fromValue)), toValue(std::move(toValue)), iterate(iterate), body(std::move(body)) {}
        llvm::Value* codegen() {}
};

#endif