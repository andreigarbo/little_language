#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

class VariableTable {

private:
    std::unique_ptr<std::unordered_map<std::string, llvm::Value*>> globalScope;
    std::stack<std::unique_ptr<std::unordered_map<std::string, llvm::Value*>>> localScopes;

    static VariableTable* instance;

    VariableTable(){
        globalScope = std::make_unique<std::unordered_map<std::string, VariableInfo>>();
    }

public:
    static VariableTable& getInstance();
    void enterScope();
    void exitScope();
    void insertVariable(const std::string& name, llvm::Value* value, llvm::Type* type);
    void setVariableValue(const std::string& name, llvm::Value* newValue);
    Value* getVariableValue(const std::string& name);
}

#endif