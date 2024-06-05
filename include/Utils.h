#ifndef LOG_ERROR_VALUE_H
#define LOG_ERROR_VALUE_H

#include "Token.h"
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <typeinfo>
#include <string>

llvm::Value *LogErrorValue(const char *Str);
bool isOperator(int token);
bool isTypeDeclaration(int token);
llvm::Value *CreateAllocaVar(llvm::Module* module, llvm::Function *currentFunction, llvm::Type* variableType, std::string VarName);

#endif