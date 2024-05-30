#include "FlowControlAST.h"
#include "ErrorPrototype.h"

llvm::Value* WhileAST::codegen(){
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    //get parent function to insert blocks
    llvm::Function* parentFunction = builder.GetInsertBlock()->getParent();

    //create the basic blocks
    llvm::BasicBlock* conditionBasicBlock = llvm::BasicBlock::Create(context, "loopcond", parentFunction);
    llvm::BasicBlock* loopBasicBlock = llvm::BasicBlock::Create(context, "loopcontent", parentFunction);
    llvm::BasicBlock* afterBasicBlock = llvm::BasicBlock::Create(context, "afterloop", parentFunction);

    //creates branch to condition
    builder.CreateBr(conditionBasicBlock);

    //set insert point to the condition block to generate code for the condition
    builder.SetInsertPoint(conditionBasicBlock);

    //generate code for condition
    llvm::Value* conditionCodegen = condition->codegen();

    if (!conditionCodegen){
        return LogErrorValue("Error generating code for condition of while loop");
    }

    //get boolean value of condition
    conditionCodegen = builder.CreateICmpNE(conditionCodegen, llvm::ConstantInt::get(context, llvm::APInt(1, 0)), "loopcond");

    //create a conditional branch to jump to either loop basic block or after basic block
    builder.CreateCondBr(conditionCodegen, loopBasicBlock, afterBasicBlock);

    //set insert point to loop body basic block
    builder.SetInsertPoint(loopBasicBlock);
    
    //generate code for the loop body
    for (auto& expression : body) {
        if (!expression->codegen()){
            return LogErrorValue("Error generating code for body of while loop");
        }
    }

    //after loop body block, jump to condition
    builder.CreateBr(conditionBasicBlock);

    //set the insertion point of the builder to the after block
    builder.SetInsertPoint(afterBasicBlock);

    //return block where next code will be inserted
    return afterBasicBlock;
}