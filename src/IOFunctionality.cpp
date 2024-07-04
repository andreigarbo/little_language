#include "LLVMState.h"
#include "IOFunctionality.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

#include <iostream>

using namespace llvm;

// void PrintGenerator::generatePrintFunction() {

//     LLVMState& llvmState = LLVMState::getInstance();
//     llvm::IRBuilder<>& builder = llvmState.getBuilder();
//     llvm::LLVMContext& context = llvmState.getContext();
//     llvm::Module& module = llvmState.getModule();

//     // Define the printf function signature: int printf(const char*, ...)
//     FunctionType *printfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);
//     Function *printfFunc = Function::Create(printfFuncType, Function::ExternalLinkage, "printf", module);

// }

void PrintGenerator::generatePrintInt() {
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::LLVMContext& context = llvmState.getContext();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::Module& module = llvmState.getModule();

    // Function type: i8* int_to_i8ptr(i32 %value)
    FunctionType *funcType = FunctionType::get(builder.getVoidTy(), {builder.getInt32Ty()}, false);
    Function *func = Function::Create(funcType, Function::ExternalLinkage, "print_int", module);

    llvmState.setCurrentFunction(func);

    // Entry basic block
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entryBB);

    // Function argument
    Argument *arg = &*(func->arg_begin());
    arg->setName("value");

    Type* arrayType = ArrayType::get(builder.getInt8Ty(), 12);

    // Create an array to store the formatted string
    Value *str = builder.CreateAlloca(arrayType, nullptr, "array.str");

    llvm::Value* indices[] = {builder.getInt32(0), builder.getInt32(0)};

    // Format the integer value as a string ("%d\n" format)
    Value *formatStr = builder.CreateGlobalStringPtr("%d\0A");

    Function *sprintfFunc = module.getFunction("sprintf");
    if (!sprintfFunc) {
        FunctionType *sprintfType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt8PtrTy()}, true);
        sprintfFunc = Function::Create(sprintfType, Function::ExternalLinkage, "sprintf", module);
    }

    Value *ptr = builder.CreateGEP(arrayType, str, indices);
    builder.CreateCall(sprintfFunc, {ptr, formatStr, arg});

    // Allocate memory for i8* (pointer to i8*)
    Value *strPtr = builder.CreateAlloca(builder.getInt8PtrTy(), nullptr, "str");

    // Cast str to i8* pointer type
    Value *strPtrCast = builder.CreateBitOrPointerCast(str, builder.getInt8PtrTy());

    // // Store the pointer to the array in strPtr
    // builder.CreateStore(strPtrCast, strPtr);

    // // Load and return the pointer to the array
    // Value *loadedStr = builder.CreateLoad(builder.getInt8PtrTy(), strPtr);

    builder.CreateCall(module.getFunction("print_string"), {strPtrCast});

    // builder.CreateRet(loadedStr);
    builder.CreateRetVoid();
}

// Define float_to_i8ptr function
void PrintGenerator::generatePrintFloat() {
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::LLVMContext& context = llvmState.getContext();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::Module& module = llvmState.getModule();


    // Function type: i8* float_to_i8ptr(float %value)
    FunctionType *funcType = FunctionType::get(builder.getInt8PtrTy(), {builder.getFloatTy()}, false);
    Function *func = Function::Create(funcType, Function::ExternalLinkage, "float_to_i8ptr", module);

    // Entry basic block
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entryBB);

    // Function arguments
    Argument *arg = &*(func->arg_begin());
    arg->setName("value");

    Type* arrayType = ArrayType::get(builder.getInt8Ty(), 20);

    // Create an array to store the formatted string
    Value *str = builder.CreateAlloca(arrayType, nullptr, "str");

    llvm::Value* indices[] = {builder.getInt32(0), builder.getInt32(0)};


    // Format the float value as a string ("%.6f\n" format)
    Value *formatStr = builder.CreateGlobalStringPtr("%.6f\0A");
    Value *ptr = builder.CreateGEP(arrayType, str, indices);

    builder.CreateCall(module.getFunction("sprintf"), {ptr, formatStr, arg});

    builder.CreateRet(str);
}

void PrintGenerator::generatePrintFunction() {
    LLVMState& llvmState = LLVMState::getInstance();
    IRBuilder<>& builder = llvmState.getBuilder();
    LLVMContext& context = llvmState.getContext();
    Module& module = llvmState.getModule();

    // Define the printf function signature: int printf(const char*, ...)
    FunctionType *printfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);
    Function *printfFunc = Function::Create(printfFuncType, Function::ExternalLinkage, "printf", module);

    FunctionType* printFunctionType = FunctionType::get(builder.getVoidTy(), {builder.getInt8PtrTy()}, true);
    Function *printFunc = Function::Create(printFunctionType, Function::ExternalLinkage, "print_string", module);

    
    // Create the entry basic block
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", printFunc);
    builder.SetInsertPoint(entryBB);

    // Get the single argument passed to the print function
    Function::arg_iterator args = printfFunc->arg_begin();
    Value *argValue = &(*args);

    Value *stringFormatStr = builder.CreateGlobalStringPtr("%s\n");
    builder.CreateCall(module.getFunction("printf"), {stringFormatStr, argValue});

 

    // Return void
    builder.CreateRetVoid();
}


void InputGenerator::generateInputFunction() {
    LLVMState& state = LLVMState::getInstance();
    IRBuilder<>& builder = state.getBuilder();
    Module& module = state.getModule();

    // Define the type for the input function (assuming i32 input() signature)
    FunctionType* inputFuncType = FunctionType::get(builder.getInt32Ty(), false);

    // Insert or get the input function
    Function* inputFunc = module.getFunction("input");
    if (!inputFunc) {
        inputFunc = Function::Create(inputFuncType, Function::ExternalLinkage, "input", module);
    }

    // Begin building the function
    BasicBlock* entry = BasicBlock::Create(module.getContext(), "entry", inputFunc);
    builder.SetInsertPoint(entry);

    // Input function implementation goes here
    // Example: use IRBuilder to create instructions

    // End the function
    builder.CreateRet(builder.getInt32(0)); // Dummy return value for example
}

void InputGenerator::generateScanfFunction() {
    LLVMState& state = LLVMState::getInstance();
    IRBuilder<>& builder = state.getBuilder();
    Module& module = state.getModule();

    // Define the type for the scanf function (assuming i32 scanf(i8*) signature)
    Type* Int32Ty = builder.getInt32Ty();
    Type* Int8PtrTy = builder.getInt8PtrTy();
    FunctionType* scanfFuncType = FunctionType::get(Int32Ty, {Int8PtrTy}, true);

    // Insert or get the scanf function
    Function* scanfFunc = module.getFunction("scanf");
    if (!scanfFunc) {
        scanfFunc = Function::Create(scanfFuncType, Function::ExternalLinkage, "scanf", module);
    }

    // Begin building the function
    BasicBlock* entry = BasicBlock::Create(module.getContext(), "entry", scanfFunc);
    builder.SetInsertPoint(entry);

    // Scanf function implementation goes here
    // Example: use IRBuilder to create instructions

    // End the function
    builder.CreateRet(builder.getInt32(0)); // Dummy return value for example
}
