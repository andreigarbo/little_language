#include "VariableAST.h"
//TODO
Value* VariableAST::codegen(){
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    //get variable table
    VariableTable& variableTable = VariableTable::getInstance();

    if (type == 0){
        //existing variable
        if (value == nullptr){
            //value reference in right side of assignment
            Value* variableValue = variableTable.getVariableValue(name);
            return builder.CreateLoad(variableValue, "loadtmp");
        }
        else {
            //assignment, variable is on left side and value is on right 
        }
    } else {
        //variable declaration statement

        if (value == nullptr){
            //variable created but not initalized
        }
        else{
            //variable create and initialized
        }
    }
}