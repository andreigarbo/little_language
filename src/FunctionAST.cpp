#include "FunctionAST.h"
#include "ErrorPrototype.h"

llvm::Value* FunctionPrototypeAST::codegen(){
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();
    llvm::Module& module = llvmState.getModule();


    //find return type
    llvm::Type* returnTypeObject;
    if (returnType == "int"){
        returnTypeObject = llvm::Type::getInt32Ty(context);
    }
    else if (returnType == "float"){
        returnTypeObject = llvm::Type::getFloatTy(context);
    }
    else if (returnType == "string"){
        // returnTypeObject = llvm::Type::
    }
    else if (returnType == "void"){
        returnTypeObject = llvm::Type::getVoidTy(context);
    }
    else{
        return LogErrorValue(("Invalid return type for function " + name).c_str());
    }

    //get function argument types
    std::vector<llvm::Type*> argumentTypeObjects;
    for (auto& arg : arguments) {
        if (dynamic_cast<FunctionPrototypeArgumentIntAST*>(arg.get())) {
            argumentTypeObjects.push_back(llvm::Type::getInt32Ty(context));
        } else if (dynamic_cast<FunctionPrototypeArgumentFloatAST*>(arg.get())) {
            argumentTypeObjects.push_back(llvm::Type::getFloatTy(context));
        } else if (dynamic_cast<FunctionPrototypeArgumentStringAST*>(arg.get())) {
            argumentTypeObjects.push_back(llvm::Type::getInt8PtrTy(context));
        } else {
            return LogErrorValue(("Unknown argument type for function " + name).c_str());
        }
    }

    //create function type
    llvm::FunctionType* funcType = llvm::FunctionType::get(returnTypeObject, argumentTypeObjects, false);
    //create the function
    llvm::Function* function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name, module);
    //set the names for the arguments
    unsigned idx = 0;
    for (auto& arg : function->args()) {
        std::unique_ptr<GenericAST>& argumentASTPtr = arguments[idx++];
        FunctionPrototypeArgumentAST* argumentAST = dynamic_cast<FunctionPrototypeArgumentAST*>(argumentASTPtr.get());
        std::string argumentASTName = argumentAST->name;
        arg.setName(argumentASTName);
    }

    return function;
}

llvm::Value* FunctionAST::codegen() {
    // //get LLVM objects
    // LLVMState& llvmState = LLVMState::getInstance();
    // llvm::LLVMContext& context = llvmState.getContext();
    // llvm::IRBuilder<>& builder = llvmState.getBuilder();

    // //codegening the function prototype
    // llvm::Function* function = dynamic_cast<llvm::Function*>(prototype->codegen());
    // if (!function) {
    //     return LogErrorValue("Error generating code for function prototype");
    // } 

    // //create a basic block for function body
    // llvm::BasicBlock* functionBodyBasicBlock = llvm::BasicBlock::Create(context, "function", function);

    // //set insert point for generating function body code
    // builder.SetInsertPoint(functionBodyBasicBlock);

    // //variable to keep track of if and where return appears
    // bool hasReturn = false;

    // //generate code
    // for(auto& expression : body){
    //     //check if GenericAST is return
    //     if (dynamic_cast<ReturnAST*>(expression.get())) {
    //         hasReturn = true;
    //     }
    //     //generating code, if error delete function
    //     if (!expression->codegen()) {
    //         function->eraseFromParent();
    //         return LogErrorValue("Error generating code for function body");
    //     }
    // }

    // //if no return encountered
    // if (!hasReturn) {
    //     //get type to check if void
    //     llvm::Type* returnType = function->getReturnType();
    //     //if no return and not void, return an error
    //     if (!returnType->isVoidTy()){
    //         return LogErrorValue("Expected explicit return statement for non-void function");
    //     }
    // }
    // //llvm function verifier
    // llvm::verifyFunction(*function);

    // return function;
    return nullptr;
}

llvm::Value* ReturnAST::codegen() {
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();

    if (returnValue) {
        llvm::Value* retVal = returnValue->codegen();
        if (!retVal) {
            return LogErrorValue("Error generating return value");
        }
        return builder.CreateRet(retVal);
    } else {
        return builder.CreateRetVoid();
    } 
}

llvm::Value* FunctionCallAST::codegen() {
    //get LLVM objects
    LLVMState& llvmState = LLVMState::getInstance();
    llvm::IRBuilder<>& builder = llvmState.getBuilder();
    llvm::LLVMContext& context = llvmState.getContext();
    llvm::Module& module = llvmState.getModule();

    //check if function exists
    llvm::Function* function = module.getFunction(callee);
    if (!function) {
        return LogErrorValue(("Reference to undeclared function: " + callee).c_str());
    }

    //check if correct no of args passed
    if (args.size() != function->arg_size()){
        return LogErrorValue(("Function " + callee + " expected " + std::to_string(function->arg_size()) + " arguments but got " + std::to_string(args.size())).c_str());
    }

    //array to store generated code from arguments
    std::vector<llvm::Value*> codegenArgValues;
    
    //generating code for arguments
    for (auto& argument : args) {
        llvm::Value* codegenArgValue = argument->codegen();
        if (!codegenArgValue){
            return LogErrorValue("Error generating code for argument");
        }
        codegenArgValues.push_back(codegenArgValue);
    }

    //return the function call
    return builder.CreateCall(function, codegenArgValues, "calltmp");
}

llvm::Value* FunctionPrototypeArgumentAST::codegen(){
    return nullptr;
}

llvm::Value* FunctionPrototypeArgumentCharAST::codegen(){
    return nullptr;
}

llvm::Value* FunctionPrototypeArgumentIntAST::codegen(){
    return nullptr;
}

llvm::Value* FunctionPrototypeArgumentFloatAST::codegen(){
    return nullptr;
}

llvm::Value* FunctionPrototypeArgumentStringAST::codegen(){
    return nullptr;
}

