#ifndef BINARY_EXP_AST
#define BINARY_EXP_AST

#include "../headers/llvm_commons.h"

class BinaryExprAST : public GenericAST {
    char operation;
    std::unique_ptr<GenericAST> leftMember, rightMember;

    public:
        BinaryExprAST(
            char operation,
            std::unique_ptr<GenericAST> leftMember,
            std::unique_ptr<GenericAST> rightMember
        ) : operation(operation), leftMember(std::move(leftMember)), rightMember(std::move(rightMember)) {}
        llvm::Value* codegen() {
            llvm::Value* leftSide = left->codegen();
            llvm::Value* rightSide = right->codegen();
            //meaning IntAST on both sides of operation

            if (llvm::ConstantDataArray *constantString = llvm::dyn_cast<llvm::ConstantDataArray>(leftSide)) {
                std::string leftVariableName = leftSide->value;
                leftSide = VariableTable::getInstance().getVariableValue(leftVariableName);
                if (!leftSide){
                    return LogErrorValue("Reference to undefined variable " + leftVariableName);
                }
            }

            if (llvm::ConstantDataArray *constantString = llvm::dyn_cast<llvm::ConstantDataArray>(rightSide)) {
                std::string rightVariableName = rightSide->value;
                rightSide = VariableTable::getInstance().getVariableValue(rightVariableName);
                if (!rightSide){
                    return LogErrorValue("Reference to undefined variable " + rightVariableName);
                }
            }

            if(auto *leftInt = llvm::dyn_cast<llvm::ConstantInt>(leftSide) && auto *rightInt = llvm::dyn_cast<llvm::ConstantInt>(rightSide)){
                switch (op){
                    case '+':
                        return Builder->CreateAdd(leftInt, rightInt, "addtmp");
                    case '-':
                        return Builder->CreateSub(leftInt, rightInt, "subtmp");
                    case '*':
                        return Builder->CreateMul(leftInt, rightInt, "multmp");
                    case '/':
                        return Builder->CreateSDiv(leftInt, rightInt, "divtmp");
                }
            }
            //meainng FloatAST on both sides of operation
            else if (auto *leftFloat = llvm::dyn_cast<llvm::ConstantFP>(leftSide) && auto *rightFloat = llvm::dyn_cast<llvm::ConstantFP>(rightSide)){
                switch (op){
                    case '+':
                        return Builder->CreateFAdd(leftFloat, rightFloat, "addtmp");
                    case '-':
                        return Builder->CreateFSub(leftFloat, rightFloat, "subtmp");
                    case '*':
                        return Builder->CreateFMul(leftFloat, rightFloat, "multmp");
                    case '/':
                        return Builder->CreateFDiv(leftFloat, rightFloat, "divtmp");
                }
            }
            else {
                return LogErrorValue("Mismatched data types for binary epxression");
            }
            return nullptr;
        }
};

#endif