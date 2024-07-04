#include "Utils.h"
#include "ErrorPrototype.h"
#include "LLVMState.h"

#include <iostream>

bool isTypeDeclaration(int token){
    if (token == token_int || token == token_float || token == token_char || token == token_string || token == token_array || token == token_file || token == token_void) {
        return true;
    }
    return false;
}

bool isOperator(int token){
    if (token == '%' || token == '/' || token == '+' || token == '-' || token == '*'){
        return true;
    }
    return false;
}

llvm::Value *CreateAllocaVar(llvm::Module* module, llvm::Function *currentFunction, llvm::Type* variableType, std::string VarName) {
    llvm::IRBuilder<> builder(currentFunction ? &currentFunction->getEntryBlock() : &module->getFunctionList().front().getEntryBlock(),
                               currentFunction ? currentFunction->getEntryBlock().begin() : module->getFunctionList().front().getEntryBlock().begin());
    if (currentFunction) {
        return builder.CreateAlloca(variableType, nullptr, VarName);
    } else {
        return new llvm::GlobalVariable(*module, variableType, false, llvm::GlobalValue::ExternalLinkage, nullptr, VarName);
    }
}