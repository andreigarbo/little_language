#include "VariableTable.h"

VariableTable* VariableTable::instance = nullptr;

VariableTable::VariableTable() {
    globalScope = std::make_unique<std::unordered_map<std::string, llvm::Value*>>();
}

VariableTable& VariableTable::getInstance(){
    if (!instance) {
        instance = new VariableTable();
    }
    return *instance;
}

void VariableTable::enterScope(){
    localScopes.push(std::make_unique<std::unordered_map<std::string, llvm::Value*>>());
}

void VariableTable::exitScope(){
    if (!localScopes.empty()) {
        localScopes.pop();
    }
}

void VariableTable::insertVariable(const std::string& name, llvm::Value* value){
    if (!localScopes.empty()) {
        (*localScopes.top())[name] = value;
    } else {
        (*globalScope)[name] = value;
    }
}

void VariableTable::setVariableValue(const std::string& name, llvm::Value* newValue){
    for (auto it = localScopes.rbegin(); it != localScopes.rend(); ++it) {
        auto iterator = (*it)->find(name);
        if (iterator != (*it)->end()) {
            iterator->second = newValue;
            return;
        }
    }
    auto globalIter = globalScope->find(name);
    if (globalIter != globalScope->end()) {
        globalIter->second = newValue;
    }
}

Value* VariableTable::getVariableValue(const std::string& name){
    for (auto it = localScopes.rbegin(); it != localScopes.rend(); ++it) {
        auto iterator = (*it)->find(name);
        if (iterator != (*it)->end()) {
            return iterator->second;
        }
    }
    auto globalIter = globalScope->find(name);
    if (globalIter != globalScope->end()) {
        return globalIter->second;
    }
    return nullptr;
}
