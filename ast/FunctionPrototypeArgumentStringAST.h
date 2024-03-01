#ifndef FUNCTION_PROTOTYPE_ARGUMENT_STRING_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_STRING_AST_H

#include "GenericAST.h"
#include "FunctionPrototypeArgumentAST.h"
#include "StringAST.h"

class FunctionPrototypeArgumentStringAST : public FunctionPrototypeArgumentAST {
    unique_ptr<StringAST> value;

    public:
        FunctionPrototypeArgumentStringAST(
            unqiue_ptr<StringAST> value;
        ) : value(std::move(value)) {}
        Value *codegen() {}
}

#endif