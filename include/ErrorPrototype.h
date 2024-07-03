#ifndef ERROR_PROTOTYPE_H
#define ERROR_PROTOTYPE_H

#include "GenericAST.h"
#include <typeinfo>

static std::unique_ptr<GenericAST> LogError(const char *string){
    fprintf(stderr, "Error: %s\n", string);
    exit(1);
    return nullptr;
}

static std::unique_ptr<GenericAST> LogErrorPrototype(const char *string){
    LogError(string);
    return nullptr;
}

static std::vector<std::unique_ptr<GenericAST>> LogErrorPrototypeVector(const char *string){
    LogError(string);
    return {};
}

#endif