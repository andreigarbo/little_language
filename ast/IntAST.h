#ifndef INT_AST
#define INT_AST

#include <GenericAST.h>

class IntAST : public GenericAST {
    int value;

    public:
        IntAST(int value) : value(value) {}
        Value *codegen() {return nullptr;}

};  

#endif