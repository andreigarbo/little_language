#ifndef ERROR_VALUE_H
#define ERROR_VALUE_H

#include <llvm/IR/Value.h>
#include <typeinfo>

llvm::Value *LogErrorValue(const char *string);

#endif