#ifndef FUNCTION_PROTOTYPE_ARGUMENT_INT_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_INT_AST_H

#include "GenericAST.h"
#include "FunctionPrototypeArgumentAST.h"
#include "IntAST.h"

class FunctionPrototypeArgumentIntAST : public FunctionPrototypeArgumentAST {
    unique_ptr<IntAST> value;

    public:
        FunctionPrototypeArgumentIntAST(
            unqiue_ptr<IntAST> value;
        ) : value(std::move(value)) {}
        Value *codegen() {}
}

#endif