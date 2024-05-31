#ifndef LLVM_STATE_H
#define LLVM_STATE_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <string>
#include <llvm/IR/Value.h>
#include <map>
#include <typeinfo>

class LLVMState {
public:
    static LLVMState& getInstance() {
        static LLVMState instance;
        return instance;
    }

    llvm::LLVMContext& getContext();
    llvm::IRBuilder<>& getBuilder();
    llvm::Module& getModule();
    std::map<std::string, llvm::Value*>& getNamedValues();
    void createModule(const std::string &moduleName);
    void printModule();

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
};

#endif