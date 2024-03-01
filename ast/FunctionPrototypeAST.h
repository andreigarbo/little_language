#ifndef PROTOTYPE_AST
#define PROTOTYPE_AST

#include <GenericAST.h>

class FunctionPrototypeAST : public GenericAST {
    std::string returnType;
    std::string name;
    std::vector<GenericAST> args;

    public:
        PrototypeAST(
            std::string returnType,
            std::string name,
            std::vector<GenericAST> args;
        ) : returnType(returnType), name(name), args(std::move(args)) {}
        Value *codegen() {}
}

#endif