#ifndef GENERIC_AST_H
#define GENERIC_AST_H

class GenericAST {
    public:
        virtual ~GenericAST() = default;
        virtual llvm::Value *codegen() = 0;
};

#endif