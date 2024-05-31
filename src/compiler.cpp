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

    llvmState.createModule("littleModule");


    for (auto& a : ast) {
        std::cout<< typeid(*a).name() << std::endl;
        a->codegen();
    }

    std::cout << "Generated code\n";

    llvmState.printModule();


    return 0;
}