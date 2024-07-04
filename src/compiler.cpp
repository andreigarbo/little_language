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

#include "IOFunctionality.h"

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

    std::cout << "Parsed file " + filename + "\n";

    LLVMState &llvmState = LLVMState::getInstance();

    //generating IR for IO
    
    PrintGenerator printGenerator;
    InputGenerator inputGenerator;

    printGenerator.generatePrintFunction();
    printGenerator.generatePrintInt();
    printGenerator.generatePrintFloat();
    inputGenerator.generateInputInt();
    inputGenerator.generateInputFloat();
    inputGenerator.generateInputString();
   


    Parser parser(filename);
    auto ast = parser.ParseFile();

    for (auto& a : ast) {
        // std::cout<< typeid(*a).name() << std::endl;
        a->codegen();
    }

    std::cout << "\nGenerated code\n\n";

    llvmState.printModule();
    llvmState.printModuleToFile("interm.ll");

    int ret;

    ret = system("llc -filetype=obj interm.ll -o interm.o");
    if (ret != 0) {
        std::cerr << "Error generating object file" << std::endl;
        std::remove("interm.ll");
        return ret;
    }

    std::string baseFilename = filename.substr(0, filename.find_last_of('.'));
    std::string clangCommand = "clang -no-pie interm.o -o" + baseFilename;

    ret = system(clangCommand.c_str());
    if (ret != 0) {
        std::remove("interm.ll");
        std::remove("interm.o");
        std::cerr << "Linker error" << std::endl;
        return ret;
    }

    std::remove("interm.ll");

    std::remove("interm.o");

    std::cout << "File " << filename << " compiled to executable ./" << baseFilename << std::endl;

    return 0;
}

// USAGE
// make
// ./little_compiler test.lln
// llc -filetype=obj test.ll -o output.o
// clang output.o -o testex
// USAGE

// Compile and execute pipeline
// clear;make;./little_compiler test.lln;./test;echo Result is $?