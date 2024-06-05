#include "BinaryExprAST.h"
#include "VariableTable.h"
#include "ErrorPrototype.h"

#include <iostream>

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
        StringAST* leftMemberStringAST = dynamic_cast<StringAST*>(leftMember.get());
        if (leftMemberStringAST){
            std::string leftVariableName = leftMemberStringAST->value;
            leftSide = variableTable.getVariableValue(leftVariableName);
            if (!leftSide){
                return LogErrorValue(("Reference to undefined variable " + leftVariableName).c_str());
            }
        }
    }

    if (llvm::isa<llvm::ConstantDataArray>(rightSide)){
        StringAST* rightMemberStringAST = dynamic_cast<StringAST*>(rightMember.get());
        if (rightMemberStringAST){
            std::string rightVariableName = rightMemberStringAST->value;
            rightSide = variableTable.getVariableValue(rightVariableName);
            if (!rightSide){
                return LogErrorValue(("Reference to undefined variable " + rightVariableName).c_str());
            }
        }
    }
    
    llvm::Type* leftValueType = leftSide->getType();
    llvm::Type* rightValueType = rightSide->getType();

    //meaning int on both sides of operation
    if (leftValueType->isIntegerTy() && rightValueType->isIntegerTy()){
        llvm::Value* leftInt = builder.CreateIntCast(leftSide, llvm::Type::getInt32Ty(context), true, "int_cast");
        llvm::Value* rightInt = builder.CreateIntCast(rightSide, llvm::Type::getInt32Ty(context), true,  "int_cast");
        switch (operation){
            case '+':
                return builder.CreateAdd(leftInt, rightInt, "addtmp");
            case '-':
                return builder.CreateSub(leftInt, rightInt, "subtmp");
            case '*':
                return builder.CreateMul(leftInt, rightInt, "multmp");
            case '/':
                return builder.CreateSDiv(leftInt, rightInt, "divtmp");
            case '<':
                return builder.CreateICmpSLT(leftInt, rightInt, "cmptmp");
            case '>':
                return builder.CreateICmpSGT(leftInt, rightInt, "cmptmp");
            case token_le:
                return builder.CreateICmpSLE(leftInt, rightInt, "cmptmp");
            case token_ge:
                return builder.CreateICmpSGE(leftInt, rightInt, "cmptmp");
            case token_eq:
                return builder.CreateICmpEQ(leftInt, rightInt, "cmptmp");
            case token_ne:
                return builder.CreateICmpNE(leftInt, rightInt, "cmptmp");
        }
    }
    //meaning FloatAST on both sides of operation
    else if (leftValueType->isFloatTy() && rightValueType->isFloatTy()){
        llvm::Value* leftFloat = builder.CreateFPCast(leftSide, llvm::Type::getFloatTy(context), "fp_cast");
        llvm::Value* rightFloat = builder.CreateFPCast(rightSide, llvm::Type::getFloatTy(context), "fp_cast");
        switch (operation){
            case '+':
                return builder.CreateFAdd(leftFloat, rightFloat, "addtmp");
            case '-':
                return builder.CreateFSub(leftFloat, rightFloat, "subtmp");
            case '*':
                return builder.CreateFMul(leftFloat, rightFloat, "multmp");
            case '/':
                return builder.CreateFDiv(leftFloat, rightFloat, "divtmp");
            case '<':
                return builder.CreateFCmpULT(leftFloat, rightFloat, "cmptmp");
            case '>':
                return builder.CreateFCmpUGT(leftFloat, rightFloat, "cmptmp");
            case token_le:
                return builder.CreateFCmpULE(leftFloat, rightFloat, "cmptmp");
            case token_ge:
                return builder.CreateFCmpUGE(leftFloat, rightFloat, "cmptmp");
            case token_eq:
                return builder.CreateFCmpUEQ(leftFloat, rightFloat, "cmptmp");
            case token_ne:
                return builder.CreateFCmpUNE(leftFloat, rightFloat, "cmptmp");
        }
    }
    else {
        return LogErrorValue("Mismatched data types for binary epxression");
    }
    return nullptr;
}