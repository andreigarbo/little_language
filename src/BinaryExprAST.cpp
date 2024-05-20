#include "BinaryExprAST.h"
#include "VariableTable.h"

llvm::Value* BinaryExprAST::codegen(){
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    //get variable table
    VariableTable& variableTable = VariableTable::getInstance();

    llvm::Value* leftSide = leftMember->codegen();
    llvm::Value* rightSide = rightMember->codegen();

    if (llvm::isa<llvm::ConstantDataArray>(leftSide)){
        std::string leftVariableName = leftMember->value;
        leftSide = variableTable.getVariableValue(leftVariableName);
        if (!leftSide){
            return LogErrorValue("Reference to undefined variable " + leftVariableName);
        }
    }

    if (llvm::isa<llvm:ConstantDataArray>(rightSide)){
        std::string rightVariableName = rightMember->value;
        rightSide = variableTable.getVariableValue(rightVariableName);
        if (!rightSide){
            return LogErrorValue("Reference to undefined variable " + rightVariableName);
        }
    }
    
    llvm::Type* leftValueType = leftSide->getType();
    llvm::Type* rightValueType = rightSide->getType();

    //meaning int on both sides of operation
    if (leftValueType->isIntegerTy() && rightValueType->isIntegerTy()){
        llvm::Value* leftInt = builder.CreateIntCast(leftSide, llvm::Type::getInt32Ty(context), true);
        llvm::Value* rightInt = builder.CreateIntCast(rightSide, llvm::Type::getInt32Ty(context), true);
        switch (op){
            case '+':
                return builder.CreateAdd(leftInt, rightInt, "addtmp");
            case '-':
                return builder.CreateSub(leftInt, rightInt, "subtmp");
            case '*':
                return builder.CreateMul(leftInt, rightInt, "multmp");
            case '/':
                return builder.CreateSDiv(leftInt, rightInt, "divtmp");
        }
    }
    //meaning FloatAST on both sides of operation
    else if (leftValueType->isFloatTy() && rightValueType->isFloatTy()){
        llvm::Value* leftFloat = builder.CreateFPCast(leftSide, llvm::Type::getFloatTy(context), true);
        llvm::Value* rightFloat = builder.CreateFPCast(rightSide, llvm::Type::getFloatTy(context), true);
        switch (op){
            case '+':
                return builder.CreateFAdd(leftFloat, rightFloat, "addtmp");
            case '-':
                return builder.CreateFSub(leftFloat, rightFloat, "subtmp");
            case '*':
                return builder.CreateFMul(leftFloat, rightFloat, "multmp");
            case '/':
                return builder.CreateFDiv(leftFloat, rightFloat, "divtmp");
        }
    }
    else {
        return LogErrorValue("Mismatched data types for binary epxression");
    }
    return nullptr;
}