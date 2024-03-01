#ifndef PROTOTYPE_AST
#define PROTOTYPE_AST

#include <GenericAST.h>

class FunctionPrototypeAST : public GenericAST {
    std::string name;
    std::vector<std::string> args;

    public:
        PrototypeAST(
            std::string name,
            std::vector<std::string> args;
        ) : name(name), args(std::move(args)) {}
        Value *codegen() {}
}

#endif