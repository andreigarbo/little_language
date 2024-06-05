#ifndef LLVM_STATE_H
#define LLVM_STATE_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <string>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <map>
#include <typeinfo>
#include <stack>

class LLVMState {
public:
    static LLVMState& getInstance();

    llvm::LLVMContext& getContext();
    llvm::IRBuilder<>& getBuilder();
    llvm::Module& getModule();
    void printModule();
    llvm::Function& getCurrentFunction();
    void setCurrentFunction(llvm::Function* function);
    llvm::BasicBlock& getCurrentBasicBlock();
    void setCurrentBasicBlock(llvm::BasicBlock* basicBlock);
    void printModuleToFile(const std::string& filename);

private:
    LLVMState();
    virtual ~LLVMState() = default;
    LLVMState(const LLVMState&) = delete;
    LLVMState& operator=(const LLVMState&) = delete;
    LLVMState(LLVMState&&) = delete;
    LLVMState& operator=(LLVMState&&) = delete;

    std::unique_ptr<llvm::LLVMContext> theContext;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unique_ptr<llvm::Module> theModule;
    std::map<std::string, llvm::Value *> NamedValues;
    std::stack<std::string> functionScopes;
    llvm::Function* currentFunction;
    llvm::BasicBlock* currentBasicBlock;
};

#endif