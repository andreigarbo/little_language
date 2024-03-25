#ifndef LLVM_COMMONS_H
#define LLVM_COMMONS_H

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<IRBuilder<>> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;

#endif