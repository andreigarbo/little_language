#ifndef LOG_ERROR_VALUE_H
#define LOG_ERROR_VALUE_H

#include "./llvm_commons.h"

Value *LogErrorValue(const char *Str) {
    LogError(Str);
    return nullptr;
}

#endif