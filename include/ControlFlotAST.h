#ifndef CONTROL_FLOW_AST_H
#define CONTROL_FLOW_AST_H

#include "GenericAST.h"
#include <memory>
#include <string>
#include <llvm/IR/Value.h>

class IfAST : public GenericAST {
    std::unique_ptr<GenericAST> condition;
    std::vector<std::unique_ptr<GenericAST>> thenExpr, elseExpr;

    public:
        IfAST(
            std::unique_ptr<GenericAST> condition,
            std::vector<std::unique_ptr<GenericAST>> thenExpr,
            std::vector<std::unique_ptr<GenericAST>> elseExpr = {}
        ) : condition(std::move(condition)), thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}
        llvm::Value *codegen() override;
};

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
        llvm::Value* codegen() override;
};

class ForRangeAST : public GenericAST {
    std::unique_ptr<GenericAST> intervalStart, intervalEnd, iterator, body;

    public:
        ForRangeAST(
            std::unique_ptr<GenericAST> intervalStart,
            std::unique_ptr<GenericAST> intervalEnd,
            std::unique_ptr<GenericAST> iterator,
            std::unique_ptr<GenericAST> body
        ) : intervalStart(std::move(intervalStart)), intervalEnd(std::move(intervalEnd)), iterator(std::move(iterator)), body(std::move(body)) {}
        llvm::Value *codegen() override;
};

class WhileAST : public GenericAST {
    std::unique_ptr<GenericAST> condition;
    std::vector<std::unique_ptr<GenericAST>> body;

    public:
        WhileAST(
            std::unique_ptr<GenericAST> condition,
            std::vector<std::unique_ptr<GenericAST>> body
        ) : condition(std::move(condition)), body(std::move(body)) {}
        llvm::Value *codegen() override;
};

class DoWhileAST : public GenericAST {
    std::unique_ptr<GenericAST> condition;
    std::vector<std::unique_ptr<GenericAST>> body;


    public:
        DoWhileAST(
            std::unique_ptr<GenericAST> condition,
            std::vector<std::unique_ptr<GenericAST>> body
        ) : body(std::move(body)), condition(std::move(condition)) {}
        llvm::Value* codegen() override;
};


#endif