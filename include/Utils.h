#ifndef LOG_ERROR_VALUE_H
#define LOG_ERROR_VALUE_H

#include <llvm/IR/Value.h>

Value *LogErrorValue(const char *Str);
bool isOperator(int token);
bool isTypeDeclaration(int token);

#endif