#include "LLVMState.h"
#include "ErrorPrototype.h"

LLVMState::LLVMState()
    : theContext(std::make_unique<llvm::LLVMContext>()),
      builder(std::make_unique<llvm::IRBuilder<>>(*theContext)),
      theModule(nullptr) {}

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