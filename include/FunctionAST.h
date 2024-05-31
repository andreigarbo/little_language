#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include <memory>
#include <string>
#include <llvm/IR/Value.h>
#include <vector>
#include <typeinfo>

#include <llvm/ADT/DenseMap.h>
#include <llvm/IR/PassManager.h>
#include <utility>
#include <llvm/IR/Verifier.h>

#include "GenericAST.h"
#include "IntAST.h"
#include "FloatAST.h"
#include "StringAST.h"
#include "CharAST.h"
#include "ArrayAST.h"
#include "Token.h"
#include "LLVMState.h"
#include "ErrorValue.h"
#include "ErrorPrototype.h"


class FunctionPrototypeAST : public GenericAST {
    int returnType;
    std::string name;
    std::vector<std::unique_ptr<GenericAST>> arguments;

    public:
        FunctionPrototypeAST(
            int returnType,
            std::string name,
            std::vector<std::unique_ptr<GenericAST>> arguments
        ) : returnType(returnType), name(name), arguments(std::move(arguments)) {}
        virtual ~FunctionPrototypeAST() = default;
        llvm::Value *codegen() override;
};

class FunctionPrototypeArgumentAST : public GenericAST {
    std::unique_ptr<GenericAST> value;

    public:
        std::string name;
        FunctionPrototypeArgumentAST( 
            std::string name,
            std::unique_ptr<GenericAST> value
        ) :  name(std::move(name)), value(std::move(value)) {}
        virtual ~FunctionPrototypeArgumentAST() = default;
        llvm::Value *codegen() override;
};

class FunctionPrototypeArgumentIntAST : public FunctionPrototypeArgumentAST {
    std::string name;
    std::unique_ptr<IntAST> value;

    public:
        FunctionPrototypeArgumentIntAST(
            std::string name,
            std::unique_ptr<IntAST> value
        ) : FunctionPrototypeArgumentAST(name, std::move(value)), name(std::move(name)), value(std::move(value)) {}
        virtual ~FunctionPrototypeArgumentIntAST() = default;
        llvm::Value *codegen() override;
};

class FunctionPrototypeArgumentCharAST : public FunctionPrototypeArgumentAST {
    std::string name;
    std::unique_ptr<CharAST> value;

    public:
        FunctionPrototypeArgumentCharAST(
            std::string name,
            std::unique_ptr<CharAST> value
        ) : FunctionPrototypeArgumentAST(name, std::move(value)), name(std::move(name)), value(std::move(value)) {}
        virtual ~FunctionPrototypeArgumentCharAST() = default;
        llvm::Value *codegen() override;
};

class FunctionPrototypeArgumentFloatAST : public FunctionPrototypeArgumentAST {
    std::string name;
    std::unique_ptr<FloatAST> value;

    public:
        FunctionPrototypeArgumentFloatAST(
            std::string name,
            std::unique_ptr<FloatAST> value
        ) : FunctionPrototypeArgumentAST(name, std::move(value)), name(std::move(name)), value(std::move(value)) {}
        virtual ~FunctionPrototypeArgumentFloatAST() = default;
        llvm::Value *codegen() override;
};

class FunctionPrototypeArgumentStringAST : public FunctionPrototypeArgumentAST {
    std::string name;
    std::unique_ptr<StringAST> value;

    public:
        FunctionPrototypeArgumentStringAST(
            std::string name,
            std::unique_ptr<StringAST> value
        ) : FunctionPrototypeArgumentAST(name, std::move(value)), name(name), value(std::move(value)) {}
        #include <memory>
        virtual ~FunctionPrototypeArgumentStringAST() = default;
        llvm::Value *codegen() override;
};

class FunctionAST : public GenericAST {
    std::unique_ptr<FunctionPrototypeAST> prototype;
    std::vector<std::unique_ptr<GenericAST>> body;

    public:
        FunctionAST(
            std::unique_ptr<FunctionPrototypeAST> prototype,
            std::vector<std::unique_ptr<GenericAST>> body = {}
        ) : prototype(std::move(prototype)), body(std::move(body)) {}
        virtual ~FunctionAST() = default;
        llvm::Value* codegen() override;
};

class FunctionCallAST : public GenericAST {
    std::string callee;
    std::vector<std::unique_ptr<GenericAST>> args;

    public: 
        FunctionCallAST(
            std::string callee,
            std::vector<std::unique_ptr<GenericAST>> args
        ) : callee(std::move(callee)), args(std::move(args)) {}
        virtual ~FunctionCallAST() = default;
        llvm::Value* codegen() override;
};

class ReturnAST : public GenericAST {
    std::unique_ptr<GenericAST> returnValue;

    public:
        ReturnAST(
            std::unique_ptr<GenericAST> returnValue
        ) : returnValue(std::move(returnValue)) {}
        virtual ~ReturnAST() = default;
        llvm::Value *codegen() override;
};

static std::unique_ptr<FunctionPrototypeAST> LogErrorPrototypeFunctionPrototype(const char *string){
    LogError(string);
    return nullptr;
}

static std::unique_ptr<ReturnAST> LogErrorPrototypeReturn(const char *string){
    LogError(string);
    return nullptr;
}

#endif