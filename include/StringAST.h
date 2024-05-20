#ifndef STRING_AST_H
#define STRING_AST_H

#include "GenericAST.h"
#include <memory>
#include <llvm/IR/Value.h>

class StringAST : public GenericAST {
    std::string value;

    public:
        StringAST(
            std::string value
        ) : value(value) {}
        llvm::Value *codegen() override;
}


#endif