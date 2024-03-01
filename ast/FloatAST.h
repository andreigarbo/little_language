#ifndef FLOAT_AST
#define FLOAT_AST

#include <GenericAST.h>

class FloatAST : public GenericAST {
    float value;

    public:
        FloatAST(float value) : value(value) {}
        Value *codegen() {return nullptr;}

};

#endif