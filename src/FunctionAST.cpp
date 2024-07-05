    #include "FunctionAST.h"
    #include "ErrorPrototype.h"
    #include "Utils.h"

    #include <iostream>

    llvm::Value* FunctionPrototypeAST::codegen(){
        //get LLVM objects
        LLVMState& llvmState = LLVMState::getInstance();
        llvm::IRBuilder<>& builder = llvmState.getBuilder();
        llvm::LLVMContext& context = llvmState.getContext();
        llvm::Module& myModule = llvmState.getModule();

        VariableTable& variableTable = VariableTable::getInstance();

        //find return type
        llvm::Type* returnTypeObject;
        if (returnType == token_int){
            returnTypeObject = llvm::Type::getInt32Ty(context);
        }
        else if (returnType == token_float){
            returnTypeObject = llvm::Type::getFloatTy(context);
        }
        else if (returnType == token_string){
            returnTypeObject = llvm::Type::getInt8PtrTy(context);
        }
        else if (returnType == token_void){
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
            } else if (auto arrayArg = dynamic_cast<FunctionPrototypeArgumentArrayAST*>(arg.get())) {
                llvm::Type* elementType = nullptr;
        
                if (arrayArg->arrayType == "int") {
                    elementType = llvm::Type::getInt32Ty(context);
                } else if (arrayArg->arrayType == "float") {
                    elementType = llvm::Type::getFloatTy(context);
                } else if (arrayArg->arrayType == "string") {
                    elementType = llvm::Type::getInt8PtrTy(context);
                } else {
                    return LogErrorValue(("Unknown array element type for function " + name).c_str());
                }

                llvm::ArrayType* arrayType = llvm::ArrayType::get(elementType, arrayArg->arraySize);
                llvm::PointerType* arrayPointerType = llvm::PointerType::get(arrayType, 0);

                argumentTypeObjects.push_back(arrayPointerType);

            } else {
                return LogErrorValue(("Unknown argument type for function " + name).c_str());
            }
        }

        //create function type
        llvm::FunctionType* funcType = llvm::FunctionType::get(returnTypeObject, argumentTypeObjects, false);
        //create the function
        llvm::Function* function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name, myModule);

        //set the names for the arguments
        std::string currentFunctionName = function->getName().str();

        variableTable.enterScope(currentFunctionName);

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
        //get LLVM objects
        LLVMState& llvmState = LLVMState::getInstance();
        llvm::LLVMContext& context = llvmState.getContext();
        llvm::IRBuilder<>& builder = llvmState.getBuilder();
        llvm::Module& myModule = llvmState.getModule();

        VariableTable& variableTable = VariableTable::getInstance();
        
        //codegening the function prototype
        llvm::Function* function = llvm::dyn_cast<llvm::Function>(prototype->codegen());
        if (!function) {
            return LogErrorValue("Error generating code for function prototype");
        } 
        //set the current function to be used by other 
        llvmState.setCurrentFunction(function);

        //enter scope in variable table for new function
        std::string currentFunctionName = function->getName().str();

        //create a basic block for function body
        llvm::BasicBlock* functionBodyBasicBlock = llvm::BasicBlock::Create(context, "function", function);

        //set insert point for generating function body code
        builder.SetInsertPoint(functionBodyBasicBlock);

        //allocating variables from function prototype
        unsigned idx = 0;
        for (auto& arg : function->args()) {
            llvm::Type* argType = arg.getType();
            std::string argName = arg.getName().str();
            //this condition is true only for arrays, which need to be passed by value not reference
            //secondary condition because strings are also pointers
            if (argType->isPointerTy() && argType->getPointerElementType() != llvm::Type::getInt8Ty(context)) {
                //get the type of the elements in the array
                llvm::Type* elementType = argType->getPointerElementType();

                //get the array type
                if (llvm::ArrayType* arrayType = llvm::dyn_cast<llvm::ArrayType>(elementType)) {

                    //allocate array as internal variable of the function
                    llvm::Value* arrayAlloc = CreateAllocaVar(&myModule, function, arrayType, argName + "_array");

                    //iterate array elements
                    for (unsigned i = 0; i < arrayType->getNumElements(); ++i) {
                        //get pointer to element in original array
                        llvm::Value* elementPtr = builder.CreateGEP(arrayType, &arg, {builder.getInt32(0), builder.getInt32(i)}, "elementPtr");

                        //load its value
                        llvm::Value* element = builder.CreateLoad(arrayType->getElementType(), elementPtr, "element");

                        //get pointer to element in new array
                        llvm::Value* newElementPtr = builder.CreateGEP(arrayType, arrayAlloc, {builder.getInt32(0), builder.getInt32(i)}, "newElementPtr");

                        //store the value in new array
                        builder.CreateStore(element, newElementPtr);
                    }

                    //insert new array in variable table
                    variableTable.insertVariable(argName, arrayAlloc);
                }
            } else {
                llvm::Value* allocatedVariable = CreateAllocaVar(&myModule, function, argType, argName);
                builder.CreateStore(&arg, allocatedVariable);
                variableTable.insertVariable(argName, allocatedVariable);
            }

        }

        //variable to keep track of if and where return appears
        bool hasReturn = false;

        //generate code
        for(auto& expression : body){
            //check if GenericAST is return
            if (dynamic_cast<ReturnAST*>(expression.get())) {
                hasReturn = true;
            }
            //generating code, if error delete function
            if (!expression->codegen()) {
                function->eraseFromParent();
                return LogErrorValue("Error generating code for function body");
            }

        }

        //if no return encountered
        if (!hasReturn) {
            //get type to check if void
            llvm::Type* returnType = function->getReturnType();
            //if no return and not void, return an error
            if (!returnType->isVoidTy()){
                return LogErrorValue("Expected explicit return statement for non-void function");
            } else {
                builder.CreateRetVoid();
            }
        }

        variableTable.exitScope();
        

        // // llvm function verifier
        // if (llvm::verifyFunction(*function)) {
        //     return LogErrorValue("Function verification failed");
        // }

        return function;
        // return nullptr;
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

        //return function call, assign it to temp var if non void
        if (function->getReturnType()->isVoidTy()) {
            return builder.CreateCall(function, codegenArgValues);
        } else {
            return builder.CreateCall(function, codegenArgValues, "calltmp");
        }
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

    llvm::Value* FunctionPrototypeArgumentArrayAST::codegen(){
        return nullptr;
    }
