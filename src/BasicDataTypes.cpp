#include "IntAST.h"
#include "FloatAST.h"
#include "StringAST.h"
#include "LLVMState.h"

llvm::Value* IntAST::codegen() {
    return llvm::ConstantInt::get(LLVMState::getInstance().getContext(), llvm::APInt(32, value, true));
}

llvm::Value* FloatAST::codegen() {
    return llvm::ConstantFP::get(LLVMState::getInstance().getContext(), llvm::APFloat(value));
}

llvm::Value* StringAST::codegen() {
    return llvm::ConstantDataArray::getString(LLVMState::getInstance().getContext(), value);
}