#ifndef MUL_DIV_OP_AST
#define MUL_DIV_OP_AST

#include "../headers/llvm_commons.h"

class MulDivOpAST : public GenericAST {
    std::unique_ptr<GenericAST> left;
    char op;
    std::unique_ptr<GenericAST> right;

    public:
        MulDivOpAST(
            std::unique_ptr<GenericAST> left,
            char op,
            std::unique_ptr<GenericAST> right
        ) : left(std::move(left)), op(op), right(std::move(right)) {}
        llvm::Value* codegen() {
            Value* leftSide = left->codegen();
            Value* rightSide = right->codegen();
            if (!left_side || !right_side){
                return nullptr;
            }
            
            switch (op){
                case '*':
                    return Builder->CreateFAdd(leftSide, rightSide, "addtmp");
                case '/':
                    return Builder->CreateFSub(leftSide, rightSide, "subtmp");bluetblu
            }
        }
};

#endif