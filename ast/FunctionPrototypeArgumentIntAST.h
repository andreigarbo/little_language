#ifndef FUNCTION_PROTOTYPE_ARGUMENT_INT_AST_H
#define FUNCTION_PROTOTYPE_ARGUMENT_INT_AST_H

#include "GenericAST.h"
#include "FunctionPrototypeArgumentAST.h"
#include "IntAST.h"

class FunctionPrototypeArgumentIntAST : public FunctionPrototypeArgumentAST {
    std::string name;
    unique_ptr<IntAST> value;

    public:
        FunctionPrototypeArgumentIntAST(
            std::string name,
            unqiue_ptr<IntAST> value;
        ) : name(name), value(std::move(value)) {}
        Value *codegen() {}
}

#endif