#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include <memory>
#include <stack>
#include <llvm/IR/Value.h>
#include <unordered_map>
#include <string>
#include <typeinfo>

class VariableTable {

private:
    std::unique_ptr<std::unordered_map<std::string, llvm::Value*>> globalScope;
    std::stack<std::unique_ptr<std::unordered_map<std::string, llvm::Value*>>> localScopes;

    static VariableTable* instance;

    VariableTable(){
        globalScope = std::make_unique<std::unordered_map<std::string, llvm::Value*>>();
    }

public:
    static VariableTable& getInstance();
    virtual ~VariableTable() = default;
    void enterScope();
    void exitScope();
    void insertVariable(const std::string& name, llvm::Value* value);
    void setVariableValue(const std::string& name, llvm::Value* newValue);
    llvm::Value* getVariableValue(const std::string& name);
};

#endif