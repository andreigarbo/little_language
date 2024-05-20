#include "LLVMState.h"

LLVMState::LLVMState()
    : TheContext(std::make_unique<llvm::LLVMContext>()),
      Builder(std::make_unique<llvm::IRBuilder<>>(*theContext)) {}

llvm::LLVMContext& LLVMState::getContext() {
    return *theContext;
}

llvm::IRBuilder<>& LLVMState::getBuilder() {
    return *builder;
}

llvm::Module& LLVMState::getModule() {
    return *theModule;
}

std::map<std::string, llvm::Value*>& LLVMState::getNamedValues() {
    return NamedValues;
}

void LLVMState::createModule(const std::string &moduleName) {
    theModule = std::make_unique<llvm::Module>(moduleName, getContext());
}