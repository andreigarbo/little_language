#include "FlowControlAST.h"
#include <typeinfo>

#include "ArrayAST.h"
#include "VariableAST.h"

#include <iostream>


//TODO

llvm::Value* ForInAST::codegen() {
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    //get parent function to insert blocks
    llvm::Function* parentFunction = builder.GetInsertBlock()->getParent();

    //create the basic blocks
    llvm::BasicBlock* beforeBasicBlock = llvm::BasicBlock::Create(context, "beforeloop", parentFunction);
    llvm::BasicBlock* loopBasicBlock = llvm::BasicBlock::Create(context, "loopcontent", parentFunction);
    llvm::BasicBlock* afterBasicBlock = llvm::BasicBlock::Create(context, "afterloop", parentFunction);
    llvm::BasicBlock* conditionBasicBlock = llvm::BasicBlock::Create(context, "loopcond", parentFunction);

    VariableTable& variableTable = VariableTable::getInstance();

    ArrayAST* arrayPtr = dynamic_cast<ArrayAST*>(iterable.get());
    VariableAST* variablePtr = dynamic_cast<VariableAST*>(iterable.get());

    if (!arrayPtr && !variablePtr) {
        return LogErrorValue("Expected array type or variable type as iterable element in for loop");
    }

    builder.CreateBr(beforeBasicBlock);

    builder.SetInsertPoint(beforeBasicBlock);

    //codegen for iterable array
    llvm::Value* iterableCodegen = iterable->codegen();

    if (!iterableCodegen) {
        return LogErrorValue("Failed to generate code for iterable in for loop");
    }

    std::cout<<"here\n";

    llvm::AllocaInst* allocInstIterable = llvm::dyn_cast<llvm::AllocaInst>(iterableCodegen);
    if (!allocInstIterable) {
        return LogErrorValue("Failed to generate code for iterable in for loop");
    }

    //checking the type of the array elements
    llvm::Type* allocatedType = allocInstIterable->getAllocatedType();

    llvm::ArrayType* arrayType = llvm::dyn_cast<llvm::ArrayType>(allocatedType);
    int arrayLength = arrayType->getNumElements();
    llvm::Value* arrayLengthCodegen = llvm::ConstantInt::get(context, llvm::APInt(32, arrayLength));
    llvm::Type* arrayElementType = arrayType->getElementType();
    //check if array type
    if (arrayType) {
        llvm::Type* elementType = arrayType->getElementType();

        if (elementType->isIntegerTy()) {
            llvm::errs() << "Array element type is an integer.\n";
        } else if (elementType->isFloatTy()) {
            llvm::errs() << "Array element type is a float.\n";
        } else if (elementType->isPointerTy() && elementType->getPointerElementType()->isIntegerTy(8)) {
            llvm::errs() << "Array element type is a string.\n";
        }
    } else {return LogErrorValue("Expected iterable to be an array type in for loop");}


    //declare a varaible for iterator
    llvm::Value* intZero = llvm::ConstantInt::get(context, llvm::APInt(32, 0));
    llvm::Value* iteratorVar = builder.CreateAlloca(llvm::Type::getInt32Ty(context), nullptr, "iteratorvar");
    llvm::Value* currentArrayElement = builder.CreateAlloca(arrayElementType, nullptr, "currentelement");
    builder.CreateStore(intZero, iteratorVar);

    builder.CreateBr(conditionBasicBlock);

    builder.SetInsertPoint(conditionBasicBlock);

     //loads the loop variable
    llvm::Value* currentVal = builder.CreateLoad(llvm::Type::getInt32Ty(context), iteratorVar, "iteratorvar");
    //creates comparison between loop variable and end variable
    llvm::Value* endCond = builder.CreateICmpSLT(currentVal, arrayLengthCodegen, "loopcond");

    //gets boolean for condition
    endCond = builder.CreateICmpNE(endCond, llvm::ConstantInt::get(context, llvm::APInt(1, 0)), "loopcond");

    //create a conditional branch to jump to either loop basic block or after basic block
    builder.CreateCondBr(endCond, loopBasicBlock, afterBasicBlock);

    //generate code for the loop
    builder.SetInsertPoint(loopBasicBlock);

    llvm::Value* indices[] = {builder.getInt32(0), currentVal};

    // Create GEP to access the current element in the array
    llvm::Value* currentIndexPointer = builder.CreateGEP(arrayType, iterableCodegen, indices);

    llvm::Value* tempVar = builder.CreateLoad(arrayElementType, currentIndexPointer, "tempvar");

    variableTable.insertVariable(variableName, currentArrayElement);

    builder.CreateStore(tempVar, currentArrayElement);

    for (auto& expression : body) {
        if (!expression->codegen()){
            return LogErrorValue("Error generating code for body of while loop");
        }
    }

    llvm::Value* nextVal = builder.CreateAdd(currentVal, llvm::ConstantInt::get(context, llvm::APInt(32, 1)), "nextval");
    builder.CreateStore(nextVal, iteratorVar);

    //after loop body block, jump to condition
    builder.CreateBr(conditionBasicBlock);

    //set the insertion point of the builder to the after block
    builder.SetInsertPoint(afterBasicBlock);

    //return block where next code will be inserted
    return afterBasicBlock;

}

llvm::Value* ForRangeAST::codegen() {
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    //get parent function to insert blocks
    llvm::Function* parentFunction = builder.GetInsertBlock()->getParent();

    VariableTable& variableTable = VariableTable::getInstance();

    //create the basic blocks
    llvm::BasicBlock* beforeBasicBlock = llvm::BasicBlock::Create(context, "beforeloop", parentFunction);
    llvm::BasicBlock* loopBasicBlock = llvm::BasicBlock::Create(context, "loopcontent", parentFunction);
    llvm::BasicBlock* conditionBasicBlock = llvm::BasicBlock::Create(context, "loopcond", parentFunction);
    llvm::BasicBlock* afterBasicBlock = llvm::BasicBlock::Create(context, "afterloop", parentFunction);

    //codegen for interval start and interval end
    llvm::Value* intervalStartCodegen = intervalStart->codegen();
    if (!intervalStartCodegen){
        return LogErrorValue("Failed to generate code for interval start in for loop");
    }

    llvm::Value* intervalEndCodegen = intervalEnd->codegen();
    if (!intervalEndCodegen){
        return LogErrorValue("Failed to generate code for interval end in for loop");
    }

    builder.CreateBr(beforeBasicBlock);

    //enter the loop basic block to bind variable to the loop block
    builder.SetInsertPoint(beforeBasicBlock);

    //declare an int variable for the iterator
    llvm::Value* loopVar = builder.CreateAlloca(llvm::Type::getInt32Ty(context), 0, variableName);
    builder.CreateStore(intervalStartCodegen, loopVar);

    //insert the variable into the variable table as well
    variableTable.insertVariable(variableName, loopVar);

    builder.CreateBr(conditionBasicBlock);

    //set insert point to condition block 
    builder.SetInsertPoint(conditionBasicBlock);

    //loads the loop variable
    llvm::Value* currentVal = builder.CreateLoad(llvm::Type::getInt32Ty(context), loopVar, variableName);
    //creates comparison between loop variable and end variable
    llvm::Value* endCond = builder.CreateICmpSLE(currentVal, intervalEndCodegen, "loopcond");

    //gets boolean for condition
    endCond = builder.CreateICmpNE(endCond, llvm::ConstantInt::get(context, llvm::APInt(1, 0)), "loopcond");

    //create a conditional branch to jump to either loop basic block or after basic block
    builder.CreateCondBr(endCond, loopBasicBlock, afterBasicBlock);

    //generate code for the loop
    builder.SetInsertPoint(loopBasicBlock);

    for (auto& expression : body) {
        if (!expression->codegen()){
            return LogErrorValue("Error generating code for body of while loop");
        }
    }
    
    //before branch to condition, increment loop value
    currentVal = builder.CreateLoad(llvm::Type::getInt32Ty(context), loopVar, variableName);

    llvm::Value* nextVal = builder.CreateAdd(currentVal, llvm::ConstantInt::get(context, llvm::APInt(32, iterateBy, true)), "nextvar");

    builder.CreateStore(nextVal, loopVar);

    //after loop body block, jump to condition
    builder.CreateBr(conditionBasicBlock);

    //set the insertion point of the builder to the after block
    builder.SetInsertPoint(afterBasicBlock);

    //return block where next code will be inserted
    return afterBasicBlock;

}