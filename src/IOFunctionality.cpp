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

    //creating the print_int function
    FunctionType *funcType = FunctionType::get(builder.getVoidTy(), {builder.getInt32Ty()}, false);
    Function *func = Function::Create(funcType, Function::ExternalLinkage, "print_int", module);

    //setting it as the function in the state variable
    llvmState.setCurrentFunction(func);

    //set the insert point to the entry basic block
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entryBB);

    //we get the argument provided to the function (the int to print)
    Argument *arg = &*(func->arg_begin());
    arg->setName("value");

    //create a type for an array of 12 characters to represent an int of max 12 characters
    Type* arrayType = ArrayType::get(builder.getInt8Ty(), 12);

    //create the array
    Value *str = builder.CreateAlloca(arrayType, nullptr, "array.str");

    //index array for the GEP instruction, setting to 0 to get base pointer
    llvm::Value* indices[] = {builder.getInt32(0), builder.getInt32(0)};

    //create a pointer to a global string that has formatting information for sprintf
    Value *formatStr = builder.CreateGlobalStringPtr("%d\0A");

    //get sprintf, linked externally
    Function *sprintfFunc = module.getFunction("sprintf");
    if (!sprintfFunc) {
        FunctionType *sprintfType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt8PtrTy()}, true);
        sprintfFunc = Function::Create(sprintfType, Function::ExternalLinkage, "sprintf", module);
    }

    //create the GEP to the 0 index of the space where the string will be stored
    Value *ptr = builder.CreateGEP(arrayType, str, indices);

    //call sprintf, this stores at ptr (locaiton calculated by GEP) the number formatted as a string
    builder.CreateCall(sprintfFunc, {ptr, formatStr, arg});

    //this creates a variable of type i8* (char pointer)
    Value *strPtr = builder.CreateAlloca(builder.getInt8PtrTy(), nullptr, "str");

    //this casts str to an i8* variable
    Value *strPtrCast = builder.CreateBitOrPointerCast(str, builder.getInt8PtrTy());

    //this creates a call to print_string with the newly constructed string
    builder.CreateCall(module.getFunction("print_string"), {strPtrCast});

    // builder.CreateRet(loadedStr);
    builder.CreateRetVoid();
}

void PrintGenerator::generatePrintFloat() {
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::LLVMContext& context = llvmState.getContext();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::Module& module = llvmState.getModule();

    //creating the print_float function
    FunctionType *funcType = FunctionType::get(builder.getVoidTy(), {builder.getFloatTy()}, false);
    Function *func = Function::Create(funcType, Function::ExternalLinkage, "print_float", module);

    //setting it as the function in the state variable
    llvmState.setCurrentFunction(func);

    //set the insert point to the entry basic block
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entryBB);

    //we get the argument provided to the function (the float to print)
    Argument *arg = &*(func->arg_begin());
    arg->setName("value");

    //create a type for an array of 20 characters to represent a float of max 20 chars
    Type* arrayType = ArrayType::get(builder.getInt8Ty(), 20);

    //create the array
    Value *str = builder.CreateAlloca(arrayType, nullptr, "array.str");

    //index array for the GEP instruction, setting to 0 to get base pointer
    llvm::Value* indices[] = {builder.getInt32(0), builder.getInt32(0)};

    //create a pointer to a global string that has formatting information for sprintf
    Value *formatStr = builder.CreateGlobalStringPtr("%f\0A");

    //get sprintf, linked externally
    Function *sprintfFunc = module.getFunction("sprintf");
    if (!sprintfFunc) {
        FunctionType *sprintfType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt8PtrTy()}, true);
        sprintfFunc = Function::Create(sprintfType, Function::ExternalLinkage, "sprintf", module);
    }

    //create the GEP to the 0 index of the space where the string will be stored
    Value *ptr = builder.CreateGEP(arrayType, str, indices);


    //cast argument to double, weird requirement of LLVM
    Value *argDouble = builder.CreateFPExt(arg, builder.getDoubleTy(), "argAsDouble");


    //call sprintf, this stores at ptr (locaiton calculated by GEP) the number formatted as a string
    builder.CreateCall(sprintfFunc, {ptr, formatStr, argDouble});

    //this creates a variable of type i8* (char pointer)
    Value *strPtr = builder.CreateAlloca(builder.getInt8PtrTy(), nullptr, "str");

    //this casts str to an i8* variable
    Value *strPtrCast = builder.CreateBitOrPointerCast(str, builder.getInt8PtrTy());

    //this creates a call to print_string with the newly constructed string
    builder.CreateCall(module.getFunction("print_string"), {strPtrCast});

    // builder.CreateRet(loadedStr);
    builder.CreateRetVoid();
}

void PrintGenerator::generatePrintFunction() {
    LLVMState& llvmState = LLVMState::getInstance();
    IRBuilder<>& builder = llvmState.getBuilder();
    LLVMContext& context = llvmState.getContext();
    Module& module = llvmState.getModule();

    //basically imports printf, it links with it
    FunctionType *printfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);
    Function *printfFunc = Function::Create(printfFuncType, Function::ExternalLinkage, "printf", module);

    //creates print_string
    FunctionType* printFunctionType = FunctionType::get(builder.getVoidTy(), {builder.getInt8PtrTy()}, true);
    Function *printFunc = Function::Create(printFunctionType, Function::ExternalLinkage, "print_string", module);

    
    //creates a basic block to insert
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", printFunc);
    builder.SetInsertPoint(entryBB);

    //takes the single argument of the function (a *i8)
    Function::arg_iterator args = printfFunc->arg_begin();
    Value *argValue = &(*args);

    //creates a global string that holds formatting info
    Value *stringFormatStr = builder.CreateGlobalStringPtr("%s\n");
    //creates call to externall linked printf
    builder.CreateCall(module.getFunction("printf"), {stringFormatStr, argValue});

    //returns void
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
