#ifndef FUNCTION_PROTOTYPE_ARGUMENT_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_AST_H

#include "GenericAST.h"

class FunctionPrototypeArgumentAST : public GenericAST {
    unique_ptr<GenericAST> value;

    public:
        FunctionPrototypeArgumentAST( 
            unique_ptr<GenericAST> value;
        ) : value(std::move(value)) {}
        Value *codegen() {}
}

#endif