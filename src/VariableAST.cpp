#include "VariableAST.h"
#include "ErrorPrototype.h"

#include <iostream>

//TODO
llvm::Value* VariableAST::codegen(){
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();
    llvm::Module& myModule = llvmState.getModule();

    // std::cout << "Builder address: " << &builder << std::endl;
    // std::cout << "Context address: " << &context << std::endl;
    // std::cout << "Module name: " << myModule.getName().str() << std::endl;

    //get variable table
    VariableTable& variableTable = VariableTable::getInstance();

    if (type == 0){
        //existing variable
        if (value == nullptr){
            //value reference in right side of assignment
            llvm::Value* variableValue = variableTable.getVariableValue(name);
            if (variableValue == nullptr){
                return LogErrorValue(("rval reference to undefined variable " + name).c_str());
            }
            llvm::Type* variableType = variableValue->getType()->getPointerElementType();
            return builder.CreateLoad(variableType, variableValue, "loadtmp");
        }
        else {
            //assignment, variable is on left side and value is on right
            llvm::Value* newValue = value->codegen();
            llvm::Value* existingVariablePointer = variableTable.getVariableValue(name);
            if (!existingVariablePointer) {
                return LogErrorValue(("lval reference to undefined variable " + name).c_str());
            }
            builder.CreateStore(newValue, existingVariablePointer);
            return existingVariablePointer;
        }
    } else {
        //variable declaration statement
        llvm::Value* allocatedVariable;
        switch (type) {
            case token_int:
                allocatedVariable = CreateAllocaVar(&myModule, &(llvmState.getCurrentFunction()), llvm::Type::getInt32Ty(context), name);
                break;
            case token_float:
                allocatedVariable = CreateAllocaVar(&myModule, &(llvmState.getCurrentFunction()), llvm::Type::getFloatTy(context), name);
                break;
            case token_string:
                allocatedVariable = CreateAllocaVar(&myModule, &(llvmState.getCurrentFunction()), llvm::Type::getInt8PtrTy(context), name);
        }
        if (value == nullptr){
            if (type == token_array) {
                return LogErrorValue("Arrays need to be initialized when created");
            }
            //variable created but not initalized
            variableTable.insertVariable(name, allocatedVariable);
            return allocatedVariable;
        }
        else{                    
            //variable create and initialized
            llvm::Value* codegenValue = value->codegen();
            if (!codegenValue){
                return LogErrorValue(("Error during assignment of variable " + name).c_str());
            }
            if (type != token_array) {
                builder.CreateStore(codegenValue, allocatedVariable);
                variableTable.insertVariable(name, allocatedVariable);
                return allocatedVariable;
            } else {
                variableTable.insertVariable(name, codegenValue);
                return codegenValue;
            }
        }
    }
}
