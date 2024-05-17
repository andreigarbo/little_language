#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include <string>
#include <stack>
#include <unordered_map>

class VariableTable {
private:
    std::unique_ptr<llvm::ValueSymbolTable> globalScope;
    std::stack<std::unique_ptr<llvm::ValueSymbolTable>> localScopes;

    // Singleton instance
    static VariableTable* instance;

    // Private constructor for singleton pattern
    VariableTable() {
        // Create the global scope
        globalScope = std::make_unique<llvm::ValueSymbolTable>();
    }

public:
    // Get the singleton instance
    static VariableTable& getInstance() {
        if (!instance) {
            instance = new VariableTable();
        }
        return *instance;
    }

    // Enter a new local scope
    void enterScope() {
        localScopes.push(std::make_unique<llvm::ValueSymbolTable>());
    }

    // Exit the current local scope
    void exitScope() {
        if (!localScopes.empty()) {
            localScopes.pop();
        }
    }

    // Insert a variable into the current scope
    void insertVariable(const std::string& name, llvm::Value* value) {
        if (!localScopes.empty() && value) {
            localScopes.top()->insert(value);
        } else if (value) {
            globalScope->insert(value); // If no local scope, insert into global
        }
    }

    // Retrieve a variable's value
    llvm::Value* getVariableValue(const std::string& name) {
        // Check local scopes from innermost to outermost
        for (auto it = localScopes.rbegin(); it != localScopes.rend(); ++it) {
            if (auto* value = (*it)->lookup(name)) {
                return value;
            }
        }
        // If not found in local scopes, check the global scope
        return globalScope->lookup(name);
    }
};

// Initialize the singleton instance to nullptr
VariableTable* VariableTable::instance = nullptr;

#endif
