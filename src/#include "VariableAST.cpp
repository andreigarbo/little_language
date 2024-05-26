#include "VariableAST.h"

llvm::Value* VariableAST::codegen() {
    // Get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();

    // Get variable table
    VariableTable& variableTable = VariableTable::getInstance();

    if (type == 0) {
        // Existing variable
        if (value == nullptr) {
            // Value reference on the right side of assignment
            llvm::Value* variablePtr = variableTable.getVariableValue(name);
            if (!variablePtr) {
                return LogErrorV("Unknown variable name: " + name);
            }
            return builder.CreateLoad(variablePtr, "loadtmp");
        } else {
            // Assignment: variable is on the left side and value is on the right
            llvm::Value* newValue = value->codegen(); // Generate LLVM IR for the value expression
            llvm::Value* variablePtr = variableTable.getVariableValue(name); // Get the pointer to the variable
            if (!variablePtr) {
                // Variable not found, create a new one
                llvm::AllocaInst* allocaInst = builder.CreateAlloca(newValue->getType(), nullptr, name); // Allocate memory for the variable
                variableTable.insertVariable(name, allocaInst); // Insert the variable into the table
                variablePtr = allocaInst; // Update the variable pointer
            }
            builder.CreateStore(newValue, variablePtr); // Store the computed value into the variable
            return newValue; // Return the assigned value
        }
    } else {
        // Variable declaration statement
        if (value == nullptr) {
            // Variable created but not initialized
            llvm::AllocaInst* allocaInst = builder.CreateAlloca(type, nullptr, name);
            variableTable.insertVariable(name, allocaInst);
            return allocaInst;
        } else {
            // Variable created and initialized
            llvm::Value* initialValue = value->codegen(); // Generate LLVM IR for the initial value
            llvm::AllocaInst* allocaInst = builder.CreateAlloca(initialValue->getType(), nullptr, name); // Allocate memory for the variable
            builder.CreateStore(initialValue, allocaInst); // Store the initial value into the variable
            variableTable.insertVariable(name, allocaInst); // Insert the variable into the table
            return initialValue; // Return the initial value
        }
    }
}
