#ifndef CHAR_AST
#define CHAR_AST

#include "llvm/IR/Value.h"
#include <GenericAST.h>

class CharAST : public GenericAST {
    char value;

    public:
        CharAST(char value) : value(value) {}
        Value *codegen() {return nullptr;}
};

#endif