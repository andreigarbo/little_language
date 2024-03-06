#ifndef FUNCTION_PROTOTYPE_ARGUMENT_INT_FLOAT_H
#define FUNCTION_PROTOTYPE_ARGUMENT_INT_FLOAT_H

#include "GenericAST.h"
#include "FunctionPrototypeArgumentAST.h"
#include "FloatAST.h"

class FunctionPrototypeArgumentFloatAST : public FunctionPrototypeArgumentAST {
    std::string name,
    unique_ptr<FloatAST> value;

    public:
        FunctionPrototypeArgumentFloatAST(
            std::string name,
            unqiue_ptr<FloatAST> value;
        ) : name(name), value(std::move(value)) {}
        Value *codegen() {}
}

#endif