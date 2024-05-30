#include "VariableAST.h"
#include "ErrorPrototype.h"

//TODO
llvm::Value* VariableAST::codegen(){
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
            llvm::Value* variableValue = variableTable.getVariableValue(name);
            llvm::Type* variableType = variableValue->getType()->getPointerElementType();
            return builder.CreateLoad(variableType, variableValue, "loadtmp");
        }
        else {
            //assignment, variable is on left side and value is on right
            llvm::Value* newValue = value->codegen();
            llvm::Value* existingVariablePointer = variableTable.getVariableValue(name);
            if (!existingVariablePointer) {
                return LogErrorValue(("Reference to undefined variable " + name).c_str());
            }
            builder.CreateStore(newValue, existingVariablePointer);
            return existingVariablePointer;
        }
    } else {
        //variable declaration statement
        llvm::AllocaInst* allocaInst;
        switch (type) {
            case token_int:
                allocaInst = builder.CreateAlloca(llvm::Type::getInt32Ty(context), nullptr, name);
                break;
            case token_float:
                allocaInst = builder.CreateAlloca(llvm::Type::getFloatTy(context), nullptr, name);
                break;
            case token_string:
                allocaInst = builder.CreateAlloca(llvm::Type::getInt8PtrTy(context), nullptr, name);

                // llvm::AllocaInst* allocaInst = builder.CreateAlloca(strConstant->getType(), nullptr, name);
                // builder.CreateStore(strConstant, allocaInst);
                // llvm::Value* strPointer = builder.CreateLoad(allocaInst);
        }
        if (value == nullptr){
            //variable created but not initalized
            return allocaInst;
        }
        else{
            //variable create and initialized
            llvm::Value* codegenValue = value->codegen();
            if (!codegenValue){
                return LogErrorValue(("Error during assignment of variable " + name).c_str());
            }
            builder.CreateStore(codegenValue, allocaInst);
            variableTable.insertVariable(name, allocaInst);
            return allocaInst;
        }
    }
}