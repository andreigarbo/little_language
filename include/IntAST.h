#ifndef INT_AST_H
#define INT_AST_H

#include <memory>
#include <llvm/IR/Value.h>

class IntAST : public GenericAST {
    int value;

    public:
        IntAST(
            int value
        ) : value(value) {}
        llvm::Value *codegen() override;
};

#endif