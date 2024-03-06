#ifndef FUNCTION_PROTOTYPE_ARGUMENT_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_AST_H

#include "GenericAST.h"

class FunctionPrototypeArgumentAST : public GenericAST {
    std::string name;
    unique_ptr<GenericAST> value;

    public:
        FunctionPrototypeArgumentAST( 
            std::string name,
            unique_ptr<GenericAST> value;
        ) : name(name), value(std::move(value)) {}
        Value *codegen() {}
}

#endif