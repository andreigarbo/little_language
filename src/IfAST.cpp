#include "FlowControlAST.h"

llvm::Value* IfAST::codegen(){
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    //get variable table
    VariableTable& variableTable = VariableTable::getInstance();

    llvm::Value* conditionCodegen = condition->codegen();

    if (!conditionCodegen){
        return LogErrorValue("Failed generating code for  condition of if...else loop");
    }

    //create comp operation
    conditionCodegen = builder.CreateICmpNE(conditionCodegen, llvm::ConstantInt::get(context, llvm::APInt(1, 0)), "ifcond");

    //get parent function to insert blocks
    llvm::Function* parentFunction = builder.GetInsertBlock()->GetParent();

    //create blocks for then, else and the merge
    llvm::BasicBlock* thenBasicBlock = llvm::BasicBlock::Create(context, "then", parentFunction);
    llvm::BasicBlock* elseBasicBlock = llvm::BasicBlock::Create(context, "else");
    llvm::BasicBlock* mergeBasicBlock = llvm::BasicBlock::Create(context, "ifcont");

    //creating a conditional branch
    builder.CreateCondBr(conditionCodegen, thenBasicBlock, elseBasicBlock);

    //set insert point to insert code for expressions in then block in the then basic block
    builder.SetInsertPoint(thenBasicBlock);
    //iterate over then AST node array and generate code for each one
    for (auto& expression : thenExpr) {
        if (!expression->codegen()){
            return LogErrorValue("Error generating code for then block of if...else condition");
        }
    }
    //create jump to the merge block
    builder.CreateBr(mergeBasicBlock);
    
    thenBasicBlock = builder.GetInsertBlock();

    //insert else basic block in list
    function->getBasicBlockList().push_back(elseBasicBlock);

    //set insert point to else block
    builder.SetInsertPoint(elseBasicBlock);
    //iterate over ASt node array and generate else code
    for (auto& expression : elseExpr) {
        if (!expression->codegen()){
            return LogErrorValue("Error generating code for else block of if...else condition");
        }
    }
    //create jump to merge block
    builder.CreateBr(mergeBasicBlock);
    
    elseBasicBlock = builder.GetInsertBlock();

    //adding merge block to block list
    function->getBasicBlockList().push_back(mergeBasicBlock);
    builder.SetInsertPoint(mergeBasicBlock);
     
    return mergeBasicBlock;
}