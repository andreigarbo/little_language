#include "VariableTable.h"

#include <iostream>

//TODO: setVariableValue and getVariableValue
//need these for VariableAST.cpp

VariableTable* VariableTable::instance = nullptr;

VariableTable& VariableTable::getInstance(){
    if (!instance) {
        instance = new VariableTable();
    }
    return *instance;
}

void VariableTable::enterScope(std::string& name) {
    auto it = localScopes->find(name);
    if (it == localScopes->end()) {
        auto newScope = std::make_unique<std::unordered_map<std::string, llvm::Value*>>();
        currentScope = newScope.get();
        (*localScopes)[name] = std::move(newScope);
    } else {
        currentScope = it->second.get();
    }
}

void VariableTable::exitScope(){
    currentScope = nullptr;
}

void VariableTable::insertVariable(const std::string& name, llvm::Value* value){
    if (currentScope != nullptr) {
        (*currentScope)[name] = value;
    } else {
        (*globalScope)[name] = value;
    }
}

void VariableTable::setVariableValue(const std::string& name, llvm::Value* newValue){
    if (currentScope != nullptr) {
        (*currentScope)[name] = newValue;
    } else {
        (*globalScope)[name] = newValue;
    }
}

llvm::Value* VariableTable::getVariableValue(const std::string& name){
    llvm::Value* varValue = (*currentScope)[name];
    if (varValue == nullptr) {
        varValue = (*globalScope)[name];
        if (varValue == nullptr) {
            return nullptr;
        }
        return varValue;
    }
    return varValue;
}



