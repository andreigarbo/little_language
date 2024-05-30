#ifndef FLOW_CONTROL_AST_H
#define FLOW_CONTROL_AST_H

#include "GenericAST.h"
#include <memory>
#include <string>
#include <typeinfo>
#include <llvm/IR/Value.h>
#include "Token.h"
#include "LLVMState.h"
#include "ErrorPrototype.h"
#include "ErrorValue.h"
#include "VariableTable.h"

class IfAST : public GenericAST {
    std::unique_ptr<GenericAST> condition;
    std::vector<std::unique_ptr<GenericAST>> thenExpr, elseExpr;

    public:
        IfAST(
            std::unique_ptr<GenericAST> condition,
            std::vector<std::unique_ptr<GenericAST>> thenExpr,
            std::vector<std::unique_ptr<GenericAST>> elseExpr = {}
        ) : condition(std::move(condition)), thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}
        virtual ~IfAST() = default;
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
        virtual ~ForInAST() = default;
        llvm::Value* codegen() override;
};

class ForRangeAST : public GenericAST {
    std::string variableName;
    int iterateBy;
    std::unique_ptr<GenericAST> intervalStart, intervalEnd;
    std::vector<std::unique_ptr<GenericAST>> body;

    public:
        ForRangeAST(
            std::string variableName,
            std::unique_ptr<GenericAST> intervalStart,
            std::unique_ptr<GenericAST> intervalEnd,
            int iterateBy,
            std::vector<std::unique_ptr<GenericAST>> body
        ) : variableName(std::move(variableName)), intervalStart(std::move(intervalStart)), intervalEnd(std::move(intervalEnd)), iterateBy(iterateBy), body(std::move(body)) {}
        virtual ~ForRangeAST() = default;
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
        virtual ~WhileAST() = default;
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
        virtual ~DoWhileAST() = default;
        llvm::Value* codegen() override;
};

static std::unique_ptr<IfAST> LogErrorPrototypeIf(const char *string){
    LogError(string);
    return nullptr;
}

static std::unique_ptr<WhileAST> LogErrorPrototypeWhile(const char *string){
    LogError(string);
    return nullptr;
}

static std::unique_ptr<DoWhileAST> LogErrorPrototypeDoWhile(const char *string){
    LogError(string);
    return nullptr;
}

#endif