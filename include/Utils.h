#ifndef LOG_ERROR_VALUE_H
#define LOG_ERROR_VALUE_H

#include "Token.h"
#include <llvm/IR/Value.h>
#include <typeinfo>

llvm::Value *LogErrorValue(const char *Str);
bool isOperator(int token);
bool isTypeDeclaration(int token);

#endif