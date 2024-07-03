#include "ErrorValue.h"

llvm::Value *LogErrorValue(const char *string){
    fprintf(stderr, "Error: %s\n", string);
    exit(1);
    return nullptr;
}