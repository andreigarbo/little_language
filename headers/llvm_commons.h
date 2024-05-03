#ifndef LLVM_COMMONS_H
#define LLVM_COMMONS_H

#include <llvm-14/llvm/IR/Value.h>
#include <llvm-14/llvm/IR/IRBuilder.h>

static std::unique_ptr<llvm::LLVMContext> TheContext = std::make_unique<llvm::LLVMContext>();
static std::unique_ptr<llvm::IRBuilder<>> Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::Value *> NamedValues;

#endif