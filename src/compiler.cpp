#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <utility>
#include <fstream>
#include <cctype>
#include <stack>
#include <unordered_map>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include "GenericAST.h"

#include "IntAST.h"
#include "FloatAST.h"
#include "CharAST.h"
#include "StringAST.h"
#include "ArrayAST.h"

#include "ErrorValue.h"
#include "ErrorPrototype.h"
#include "LLVMState.h"
#include "Token.h"

#include "FunctionAST.h"
#include "FlowControlAST.h"
#include "ImportAST.h"
#include "VariableAST.h"
#include "BinaryExprAST.h"

#include "Lexer.h"
#include "Parser.h"


int main(int ac, char** argv) {

    if (ac != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return -1;
    }

    std::string filename = argv[1];

    if (FILE *file = fopen(filename.c_str(), "r")) {
        fclose(file);
    } else {
        std::cerr << "File " + filename + " not found" << std::endl;
        return -1;
    }

    Parser parser(filename);
    auto ast = parser.ParseFile();

    std::cout << "Parsed file " + filename + "\n";

    LLVMState &llvmState = LLVMState::getInstance(); 

    for (auto& a : ast) {
        std::cout<< typeid(*a).name() << std::endl;
        a->codegen();
    }

    std::cout << "Generated code\n";

    llvmState.printModule();
    llvmState.printModuleToFile("interm.ll");

    return 0;
}

// int main() {
//     // Create a new LLVM context
//     llvm::LLVMContext context;

//     // Create a new module
//     std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>("my_module", context);

//     // // Create a function signature: int foo()
//     llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
//     llvm::Function *fooFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "foo", module.get());

//     // Create a basic block and add it to the function
//     llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", fooFunc);

//     // Create an IRBuilder and set its insertion point to the basic block
//     llvm::IRBuilder<> builder(entryBlock);

//     // Create an alloca instruction in the entry block of the function
//     llvm::AllocaInst *allocaInst = builder.CreateAlloca(llvm::Type::getInt32Ty(context), nullptr, "myVariable");

//     // Return 0
//     builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0));

//     // Verify the function
//     // llvm::verifyFunction(*fooFunc);

//     // Print the generated LLVM IR to the console
//     module->print(llvm::outs(), nullptr);

//     return 0;
// }