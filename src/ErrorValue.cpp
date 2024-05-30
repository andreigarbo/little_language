#include "ErrorValue.h"

llvm::Value *LogErrorValue(const char *string){
    fprintf(stderr, "Error: %s\n", string);
    return nullptr;
}