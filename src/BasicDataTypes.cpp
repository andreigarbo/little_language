#include "IntAST.h"
#include "FloatAST.h"
#include "StringAST.h"
#include "ArrayAST.h"
#include "CharAST.h"
#include "LLVMState.h"
#include "ErrorValue.h"
#include "ErrorPrototype.h"
#include "LLVMState.h"

#include <iostream>

llvm::Value* IntAST::codegen() {
    return llvm::ConstantInt::get(LLVMState::getInstance().getContext(), llvm::APInt(32, value, true));
}

llvm::Value* FloatAST::codegen() {
    return llvm::ConstantFP::get(LLVMState::getInstance().getContext(), llvm::APFloat(value));
}

llvm::Value* StringAST::codegen() {
    return llvm::ConstantDataArray::getString(LLVMState::getInstance().getContext(), value);
}

llvm::Value* ArrayAST::codegen() {
    std::vector<llvm::Constant *> constElements;
    llvm::Type *elementType = nullptr;

    for (auto &element : value) {
        llvm::Value *elementValue = element->codegen();
        if (!elementValue)
            return nullptr;

        if (!elementType)
            elementType = elementValue->getType();
        else if (elementType != elementValue->getType())
            return LogErrorValue("Array elements must have the same type");

        if (llvm::Constant *constElement = llvm::dyn_cast<llvm::Constant>(elementValue))
            constElements.push_back(constElement);
        else
            return LogErrorValue("Array elements must be constant values");
    }
    unsigned min_num_els = 0;
    llvm::ScalableVectorType *vectorType = llvm::ScalableVectorType::get(elementType, min_num_els);

    return llvm::ConstantVector::get(constElements);
}

//TODO
llvm::Value* CharAST::codegen() {
    return nullptr;
}