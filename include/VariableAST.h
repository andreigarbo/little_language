#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/ADT/Twine.h>


#include <memory>
#include <typeinfo>

#include "Token.h"
#include "GenericAST.h"
#include "ErrorValue.h"
#include "LLVMState.h"
#include "VariableTable.h"

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
        virtual ~VariableAST() = default;
        llvm::Value* codegen() override;
};

#endif