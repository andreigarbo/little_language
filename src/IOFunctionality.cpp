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
    Function *printfFunc = module.getFunction("sprintf");
    if (!printfFunc) {
        FunctionType *printfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);
        printfFunc = Function::Create(printfFuncType, Function::ExternalLinkage, "printf", module);
    }

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

void InputGenerator::generateInputInt() {
    LLVMState& llvmState = LLVMState::getInstance();
    IRBuilder<>& builder = llvmState.getBuilder();
    LLVMContext& context = llvmState.getContext();
    Module& module = llvmState.getModule();

    //get C function scanf
    Function *scanfFunc = module.getFunction("scanf");
    if (!scanfFunc) {
        FunctionType *scanfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);
        scanfFunc = Function::Create(scanfFuncType, Function::ExternalLinkage, "scanf", module);
    }

    //create our input_int function
    FunctionType *inputIntType = FunctionType::get(builder.getInt32Ty(), {}, true);
    Function *inputIntFunc = Function::Create(inputIntType, Function::ExternalLinkage, "read_int", module);

    //creates a basic block to insert
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", inputIntFunc);
    builder.SetInsertPoint(entryBB);

    //allocates space for the int to be read
    llvm::Value* readValueAlloca = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "read_value_alloca");

    //the format string
    llvm::Value* stringFormatStr = builder.CreateGlobalStringPtr("%d");

    //emits a call to scanf
    builder.CreateCall(module.getFunction("scanf"), {stringFormatStr, readValueAlloca});

    //loads the value from the readValueAlloca pointer
    llvm::Value* readValue = builder.CreateLoad(builder.getInt32Ty(), readValueAlloca, "read_value");

    //returns the value read from keyboard
    builder.CreateRet(readValue);
}

void InputGenerator::generateInputFloat() {
    LLVMState& llvmState = LLVMState::getInstance();
    IRBuilder<>& builder = llvmState.getBuilder();
    LLVMContext& context = llvmState.getContext();
    Module& module = llvmState.getModule();

    //get C function scanf
    Function *scanfFunc = module.getFunction("scanf");
    if (!scanfFunc) {
        FunctionType *scanfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);
        scanfFunc = Function::Create(scanfFuncType, Function::ExternalLinkage, "scanf", module);
    }

    //create our input_int function
    FunctionType *inputFloatType = FunctionType::get(builder.getFloatTy(), {}, true);
    Function *inputFloatFunc = Function::Create(inputFloatType, Function::ExternalLinkage, "read_float", module);

    //creates a basic block to insert
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", inputFloatFunc);
    builder.SetInsertPoint(entryBB);

    //allocates space for the float to be read
    llvm::Value* readValueAlloca = builder.CreateAlloca(builder.getFloatTy(), nullptr, "read_value_alloca");

    //the format string
    llvm::Value* stringFormatStr = builder.CreateGlobalStringPtr("%f");

    //emits a call to scanf
    builder.CreateCall(module.getFunction("scanf"), {stringFormatStr, readValueAlloca});

    //loads the value from the readValueAlloca pointer
    llvm::Value* readValue = builder.CreateLoad(builder.getFloatTy(), readValueAlloca, "read_value");

    //returns the value read from keyboard
    builder.CreateRet(readValue);
}

void InputGenerator::generateInputString() {
    LLVMState& llvmState = LLVMState::getInstance();
    IRBuilder<>& builder = llvmState.getBuilder();
    LLVMContext& context = llvmState.getContext();
    Module& module = llvmState.getModule();

    //get C function scanf
    Function *scanfFunc = module.getFunction("scanf");
    if (!scanfFunc) {
        FunctionType *scanfFuncType = FunctionType::get(builder.getInt8PtrTy(), {}, true);
        scanfFunc = Function::Create(scanfFuncType, Function::ExternalLinkage, "scanf", module);
    }

    //get C function strlen
    Function *strlenFunc = module.getFunction("strlen");
    if (!strlenFunc) {
        FunctionType *strlenFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, false);
        strlenFunc = Function::Create(strlenFuncType, Function::ExternalLinkage, "strlen", module);
    }

    //create our input_int function
    FunctionType *inputStringType = FunctionType::get(builder.getInt8PtrTy(), {}, true);
    Function *inputStringFunc = Function::Create(inputStringType, Function::ExternalLinkage, "read_string", module);

    //creates a basic block to insert
    BasicBlock *entryBB = BasicBlock::Create(context, "entry", inputStringFunc);
    builder.SetInsertPoint(entryBB);

    //defines a type for the buffer
    llvm::Type *bufferType = llvm::ArrayType::get(builder.getInt8Ty(), 1024);

    //allocates a buffer of the defined type
    llvm::Value *buffer = builder.CreateAlloca(bufferType, nullptr, "read_value_buffer");

    //get pointer to buffer
    llvm::Value *bufferPtr = builder.CreateGEP(bufferType, buffer, {builder.getInt32(0), builder.getInt32(0)});
    // llvm::Value *formatStrPtr = builder.CreateGEP(formatStrGlobal->getValueType(), formatStrGlobal, {builder.getInt32(0), builder.getInt32(0)});

    //the format string
    llvm::Value* stringFormatStr = builder.CreateGlobalStringPtr("%[^\n]");

    //emits a call to scanf
    builder.CreateCall(scanfFunc, {stringFormatStr, bufferPtr});

    //emits a call to strlen to find length of string
    llvm::Value *strlenResult = builder.CreateCall(strlenFunc, {bufferPtr}, "strlen_result");

    //adding 1 for the null terminator
    llvm::Value *strlenWithNull = builder.CreateAdd(strlenResult, builder.getInt32(1));

    //creates new buffer to size of the read string
    llvm::Value *exactSizeBuffer = builder.CreateAlloca(builder.getInt8Ty(), strlenWithNull, "exact_size_buffer");

    //copies to appropriately sized buffer
    builder.CreateMemCpy(exactSizeBuffer, llvm::MaybeAlign(1), bufferPtr, llvm::MaybeAlign(1), strlenWithNull);

    //returns pointer to appropriately sized buffer
    builder.CreateRet(exactSizeBuffer);
}