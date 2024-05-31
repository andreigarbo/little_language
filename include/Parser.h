#ifndef PARSER_H
#define PARSER_H

#include "GenericAST.h"
#include "ErrorPrototype.h"
#include "Token.h"
#include "Utils.h"

#include "Lexer.h"

#include "IntAST.h"
#include "FloatAST.h"
#include "StringAST.h"
#include "CharAST.h"

#include "BinaryExprAST.h"
#include "FunctionAST.h"
#include "ImportAST.h"
#include "Utils.h"
#include "VariableAST.h"
#include "FlowControlAST.h"

#include <typeinfo>
#include <string>

class Parser {

    public:
        Parser(
            std::string filename
        ) : filename(std::move(filename)) {}
        virtual ~Parser() = default;
        std::vector<std::unique_ptr<GenericAST>> ParseFile();

    private:
        //lexer object
        Lexer lexer;
        //filename of file to compile
        std::string filename;
        //array of AST nodes representing file
        std::vector<std::unique_ptr<GenericAST>> ASTarray;

        //token related variables
        int current_token, next_token;
        double numeric_value_float;
        int numeric_value_int;
        std::string identifier_string;

        //data types
        std::unique_ptr<IntAST> ParseIntExpr();
        std::unique_ptr<FloatAST> ParseFloatExpr();
        std::unique_ptr<StringAST> ParseStringExpr();
        std::unique_ptr<CharAST> ParseCharExpr();
        std::unique_ptr<ArrayAST> ParseArrayExpr();

        //artihmetic expressions
        std::unique_ptr<GenericAST> ParseAddSubExpr();
        std::unique_ptr<GenericAST> ParseMulDivExpr();
        std::unique_ptr<GenericAST> ParseTermExpr();

        //conditional statements and flow control
        std::unique_ptr<IfAST> ParseIfStatementExpr();
        std::unique_ptr<WhileAST> ParseWhileStatementExpr();
        std::unique_ptr<DoWhileAST> ParseDoWhileStatementExpr();
        std::unique_ptr<GenericAST> ParseForStatementExpr();

        //function parsing related 
        std::unique_ptr<FunctionAST> ParseFunctionExpr(int functionType, std::string functionName);
        std::unique_ptr<FunctionPrototypeAST> ParseFunctionPrototypeExpr(int functionType, std::string functionName);
        std::unique_ptr<GenericAST> ParseFunctionPrototypeArgumentExpr();
        std::vector<std::unique_ptr<GenericAST>> ParseFunctionBodyExpr();

        //declaring and assigning variables
        std::unique_ptr<GenericAST> ParseDeclarationOrAssignmentExpr();
        std::unique_ptr<GenericAST> ParseGlobalVariableExpr();

        //for importing libraries
        std::unique_ptr<ImportAST> ParseImportExpr();

        //for conditions to be used with for, do...while, while, etc..
        std::unique_ptr<GenericAST> ParseConditionExpr();

        //for return
        std::unique_ptr<ReturnAST> ParseReturnExpr();


        std::unique_ptr<GenericAST> ParseStatementExpr();


        void get_next_token();
};

#endif