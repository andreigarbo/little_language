#ifndef ADD_SUB_OP_AST
#define ADD_SUB_OP_AST

#include "../headers/llvm_commons.h"

class AddSubOpAST : public GenericAST {
    std::unique_ptr<GenericAST> left;
    char op;
    std::unique_ptr<GenericAST> right;

    public:
        AddSubOpAST(
            std::unique_ptr<GenericAST> left,
            char op,
            std::unique_ptr<GenericAST> right
        ) : left(std::move(left)), op(op), right(std::move(right)) {}
        llvm::Value* codegen() { // ((2+3) - (2-3))
            Value* leftSide = left->codegen();
            Value* rightSide = right->codegen();
            if (!left_side || !right_side){
                return nullptr;
            }
            //meaning IntAST on both sides of operation
            if(auto *leftInt = dynamic_cast<IntAST*>(left.get()) && auto *rightInt = dynamic_cast<IntAST*>(right.get())){
                int leftIntValue = leftInt->value;
                int rightIntValue = rightInt->value;
                switch (op){
                    case '+':
                        return Builder->CreateAdd(leftIntValue, rightIntValue, "addtmp");
                    case '-':
                        return Builder->CreateSub(leftIntValue, rightIntValue, "subtmp");
                }
            }
            //meainng FloatAST on both sides of operation
            else if (auto *leftFloat = dynamic_cast<FloatAST*>(left.get()) && auto *rightFloat = dynamic_cast<FloatAST*>(right.get())){
                float leftFloatValue = leftFloat->value;
                float rightFloatValue = rightFloat->value;
                switch (op){
                    case '+':
                        return Builder->CreateFAdd(leftFloatValue, rightFloatValue, "addtmp");
                    case '-':
                        return Builder->CreateFSub(leftFloatValue, rightFloatValue, "subtmp");
                }
            }
            //we have a varialbe, either on the left or on the right side (or both)
            //TODO: create a varialbe table, preferrably a data structure to allow scoping
            //fiels -> name, type, value
            else if (auto *leftString = dynamic_cast<StringAST>(left.get()) || auto *rightString = dynamic_cast<StringAST>(right.get())){
                bool leftIsVariable = false;
                bool rightIsVariable = false;
                if (auto *leftString = dynamic_cast<StringAST>(left.get())){
                    leftIsVariable = true;
                }
                if (auto *rightString = dynamic_cast<StringAST>(right.get())){
                    rightIsVariable = true;
                }
                
            }
        }
};

#endif