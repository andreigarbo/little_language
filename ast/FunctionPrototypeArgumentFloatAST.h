#ifndef FUNCTION_PROTOTYPE_ARGUMENT_INT_FLOAT_H
#define FUNCTION_PROTOTYPE_ARGUMENT_INT_FLOAT_H

#include "GenericAST.h"
#include "FunctionPrototypeArgumentAST.h"
#include "FloatAST.h"

class FunctionPrototypeArgumentFloatAST : public FunctionPrototypeArgumentAST {
    unique_ptr<FloatAST> value;

    public:
        FunctionPrototypeArgumentFloatAST(
            unqiue_ptr<FloatAST> value;
        ) : value(std::move(value)) {}
        Value *codegen() {}
}

#endif