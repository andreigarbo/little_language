#include "LLVMState.h"
#include "ErrorPrototype.h"

#include "llvm/Support/Host.h"

LLVMState::LLVMState()
    : theContext(std::make_unique<llvm::LLVMContext>()),
      builder(std::make_unique<llvm::IRBuilder<>>(*theContext)),
      theModule(std::make_unique<llvm::Module>("MainModule", *theContext)),
      currentFunction(nullptr),
      currentBasicBlock(nullptr) {
        llvm::Triple triple(llvm::sys::getProcessTriple());
        theModule->setTargetTriple(triple.str());
      }

LLVMState& LLVMState::getInstance() {
    static LLVMState instance;
    return instance;
}

llvm::LLVMContext& LLVMState::getContext() {
    return *theContext;
}

llvm::IRBuilder<>& LLVMState::getBuilder() {
    return *builder;
}

llvm::Module& LLVMState::getModule() {
    return *theModule;
}

void LLVMState::printModule() {
    theModule->print(llvm::outs(), nullptr);
}

llvm::Function& LLVMState::getCurrentFunction() {
    return* currentFunction;
}
void LLVMState::setCurrentFunction(llvm::Function* function) {
    currentFunction = function;
}
llvm::BasicBlock& LLVMState::getCurrentBasicBlock() {
    return *currentBasicBlock;
}
void LLVMState::setCurrentBasicBlock(llvm::BasicBlock* basicBlock) {
    currentBasicBlock = basicBlock;
}

void LLVMState::printModuleToFile(const std::string& filename) {
    std::error_code EC;
    llvm::raw_fd_ostream outputFile(filename, EC);
    if (EC) {
        llvm::errs() << "Error opening file: " << EC.message() << "\n";
        return;
    }
    theModule->print(outputFile, nullptr);
    outputFile.flush();
    outputFile.close(); 
}

// void LLVMState::enterFunctionScope(std::string& function) {
//     functionScopes.push(functionName);
// }

// void LLVMState::exitFunctionScope() {
//     functionScopes.pop();
// }

// void LLVMState::getCurrentFunction() {
//     return functionScopes.top();
// }

// void LLVMState::isInFunction() {
//     return !functionScopes.empty();
// }