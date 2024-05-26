#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include "GenericAST.h"
#include <memory>
#include <string>
#include <llvm/IR/Value.h>

class FunctionPrototypeAST : public GenericAST {
    std::string returnType;
    std::string name;
    std::vector<std::unique_ptr<GenericAST>> arguments;

    public:
        FunctionPrototypeAST(
            std::string returnType,
            std::string name,
            std::vector<std::unique_ptr<GenericAST>> arguments
        ) : returnType(returnType), name(name), arguments(std::move(arguments)) {}
        llvm::Value *codegen() override;
};

class FunctionPrototypeArgumentAST : public GenericAST {
    std::string name;
    std::unique_ptr<GenericAST> value;

    public:
        FunctionPrototypeArgumentAST( 
            std::string name,
            std::unique_ptr<GenericAST> value
        ) :  name(std::move(name)), value(std::move(value)) {}
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
        llvm::Value *codegen() {}
};

class FunctionAST : public GenericAST {
    std::unique_ptr<FunctionPrototypeAST> prototype;
    std::vector<std::unique_ptr<GenericAST>> body;

    public:
        FunctionAST(
            std::unique_ptr<FunctionPrototypeAST> prototype,
            std::vector<std::unique_ptr<GenericAST>> body = {}
        ) : prototype(std::move(prototype)), body(std::move(body)) {}
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
        llvm::Value* codegen() {}
};

class ReturnAST : public GenericAST {
    std::unique_ptr<GenericAST> returnValue;

    public:
        ReturnAST(
            std::unique_ptr<GenericAST> returnValue
        ) : returnValue(std::move(returnValue)) {}
        llvm::Value *codegen() override;
};

#endif