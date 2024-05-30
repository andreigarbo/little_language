#include "FlowControlAST.h"
#include "ErrorPrototype.h"

llvm::Value* DoWhileAST::codegen(){
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    //get parent function to insert blocks
    llvm::Function* parentFunction = builder.GetInsertBlock()->getParent();

    //create the basic blocks
    llvm::BasicBlock* loopBasicBlock = llvm::BasicBlock::Create(context, "loopcontent", parentFunction);
    llvm::BasicBlock* conditionBasicBlock = llvm::BasicBlock::Create(context, "loopcond", parentFunction);
    llvm::BasicBlock* afterBasicBlock = llvm::BasicBlock::Create(context, "afterloop", parentFunction);

    //creates branch to body
    builder.CreateBr(loopBasicBlock);

    //sets insert point to loop body to generate code in that block
    builder.SetInsertPoint(loopBasicBlock);

    //generate code for the loop body
    for(auto& expression : body){
        if (!expression->codegen()){
            return LogErrorValue("Error generating code for body of do...while loop");
        }
    }

    //after body, creating branch to condition
    builder.CreateBr(conditionBasicBlock);

    //adding condition block
    parentFunction->getBasicBlockList().push_back(conditionBasicBlock);

    //set insert point to condition block to generate code
    builder.SetInsertPoint(conditionBasicBlock);

    //generate code for the condition
    llvm::Value* conditionCodegen = condition->codegen();
    if (!conditionCodegen) {
        return LogErrorValue("Failed generating code for condition of do...while loop");
    }

    //convert condition to bool
    conditionCodegen = builder.CreateICmpNE(conditionCodegen, llvm::ConstantInt::get(context, llvm::APInt(1, 0)), "loopcond");

    //create a conditional branch based on the condition to go to either after of loop
    builder.CreateCondBr(conditionCodegen, loopBasicBlock, afterBasicBlock);

    //add after block to function
    parentFunction->getBasicBlockList().push_back(afterBasicBlock);

    //set insert point to after block
    builder.SetInsertPoint(afterBasicBlock);

    //return the after block
    return afterBasicBlock;
}