#include "IntAST.h"
#include "FloatAST.h"
#include "StringAST.h"
#include "ArrayAST.h"
#include "CharAST.h"
#include "LLVMState.h"
#include "ErrorValue.h"
#include "ErrorPrototype.h"
#include "ArrayAccessAST.h"
#include "VariableTable.h"

#include <iostream>


#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>

llvm::Value* IntAST::codegen() {
    return llvm::ConstantInt::get(LLVMState::getInstance().getContext(), llvm::APInt(32, value, true));
}

llvm::Value* FloatAST::codegen() {
    return llvm::ConstantFP::get(LLVMState::getInstance().getContext(), llvm::APFloat(value));
}

llvm::Value* StringAST::codegen() {
    llvm::LLVMContext& context = LLVMState::getInstance().getContext();
    llvm::IRBuilder<>& builder = LLVMState::getInstance().getBuilder();
    llvm::Module* module = &LLVMState::getInstance().getModule(); 

    llvm::Constant* strConstant = llvm::ConstantDataArray::getString(context, value, true);

    llvm::GlobalVariable* strVar = new llvm::GlobalVariable(*module, strConstant->getType(), true, llvm::GlobalValue::PrivateLinkage, strConstant);

    llvm::Value* strPtr = builder.CreateBitCast(strVar, llvm::Type::getInt8PtrTy(context), "string_ptr");

    return strPtr;
}


llvm::Value* ArrayAST::codegen() {
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    llvm::Type* elementType = nullptr;

    if (!value.empty()) {
        llvm::Value* firstElementCodegen = value[0]->codegen();

        if (!firstElementCodegen) {
            return LogErrorValue("Arrays need to have at least one element");
        }
        elementType = firstElementCodegen->getType();
    }

    if (!elementType) {
        return LogErrorValue("Arrays need to have at least one element");
    }

    // llvm::PointerType *elementPtrType = llvm::PointerType::get(elementType, 0);
    // llvm::ArrayType *arrayType = llvm::ArrayType::get(elementPtrType, value.size());
    llvm::ArrayType *arrayType = llvm::ArrayType::get(elementType, value.size());
    llvm::Value *arrayAlloc = builder.CreateAlloca(arrayType, nullptr, "array_alloc");

    for (int i = 0; i < value.size(); i++){
        llvm::Value* currentElementValue = value[i]->codegen();

        if (!currentElementValue) {
            return LogErrorValue("Error while generating code for array element");
        }

        if (currentElementValue->getType() != elementType) {
            return LogErrorValue("Expected all array elements to have the same type");
        }

        // llvm::Value* elementAlloc = builder.CreateAlloca(currentElementValue->getType(), nullptr, "element_alloc");
        // builder.CreateStore(currentElementValue, elementAlloc);

        // llvm::Value* elementPointer = builder.CreateGEP(arrayType, arrayAlloc, {builder.getInt32(0), builder.getInt32(i)});
        // builder.CreateStore(elementAlloc, elementPointer);

        llvm::Value* elementPointer = builder.CreateGEP(arrayType, arrayAlloc, {builder.getInt32(0), builder.getInt32(i)});
        // llvm::Value* pointerToElement = builder.CreateLoad(elementPointer->getType()->getPointerElementType(), elementPointer);
        builder.CreateStore(currentElementValue, elementPointer);
    }

    return arrayAlloc;

}

// llvm::Value* ArrayAccessAST::codegen() {
//     LLVMState& llvmState = LLVMState::getInstance();
//     llvm::LLVMContext& context = llvmState.getContext();
//     llvm::IRBuilder<>& builder = llvmState.getBuilder();
//     llvm::Module& myModule = llvmState.getModule();

//     VariableTable& variableTable = VariableTable::getInstance();

//     if (!variableTable.getVariableValue(array_name)){
//         return LogErrorValue(("Array " + array_name + " does not exist or not initialized").c_str());
//     }

//     llvm::Value* arrayAlloc = variableTable.getVariableValue(array_name);
//     llvm::ArrayType* arrayType = llvm::cast<llvm::ArrayType>(arrayAlloc->getType()->getPointerElementType());

//     llvm::Value* elementPointerPtr = builder.CreateGEP(arrayType, arrayAlloc, {builder.getInt32(0), builder.getInt32(array_index)});
//     llvm::Value* elementPointer = builder.CreateLoad(elementPointerPtr->getType()->getPointerElementType(), elementPointerPtr);

//     if (store){
//         llvm::Value* codegenValue = value->codegen();
//         return builder.CreateStore(codegenValue, elementPointer);
//     } else {
//         llvm::Value* loadedValue = builder.CreateLoad(elementPointer->getType()->getPointerElementType(), elementPointer);
//         return loadedValue;
//     }
// }

llvm::Value* ArrayAccessAST::codegen() {
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::LLVMContext& context = llvmState.getContext();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::Module& myModule = llvmState.getModule();

    VariableTable& variableTable = VariableTable::getInstance();

    llvm::Value* arrayIndexCodegen = array_index->codegen();

    if (!variableTable.getVariableValue(array_name)){
        return LogErrorValue(("Array " + array_name + " does not exist or not initialized").c_str());
    }

    llvm::Value* arrayAlloc = variableTable.getVariableValue(array_name);
    llvm::ArrayType* arrayType = llvm::cast<llvm::ArrayType>(arrayAlloc->getType()->getPointerElementType());

    llvm::Value* indices[] = {llvm::ConstantInt::get(context, llvm::APInt(32, 0)), arrayIndexCodegen};

    llvm::Value* elementPointer = builder.CreateGEP(arrayType, arrayAlloc, indices);

    if (store){
        llvm::Value* codegenValue = value->codegen();
        return builder.CreateStore(codegenValue, elementPointer);
    } else {
        llvm::Value* loadedValue = builder.CreateLoad(elementPointer->getType()->getPointerElementType(), elementPointer);
        return loadedValue;
    }

}


//TODO
llvm::Value* CharAST::codegen() {
    return nullptr;
}