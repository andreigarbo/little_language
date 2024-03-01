#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "headers/parser_headers.h"
#include "headers/shared_memory.h"

static int current_token;
static int get_next_token(){
    return current_token = gettok();
}

static std::unique_ptr<GenericAST> LogError(const char *string){
    fprintf(stderr, "Error: %s\n", string);
    return nullptr;
}

static std::unique_ptr<FunctionPrototypeAST> LogErrorPrototype(const char *string){
    LogError(string);
    return nullptr;
}

static std::unique_ptr<GenericAST> ParseIntExpr(){
    auto value = std::make_unique<IntAST>(numeric_value_int);
    get_next_token();
    return std::move(value);
}

static std::unique_ptr<GenericAST> ParseFloatExpr(){
    auto value = std::make_unique<FloatAST>(numeric_value_float);
    get_next_token();
    return std::move(value);
}

static std::unique_ptr<GenericAST> ParseStringExpr(){
    auto value = std::make_unique<StringAST>(identifier_string);
    get_next_token();
    return std::move(value);
}

static std::unique_ptr<GenericAST> ParseBinaryExpr(){
    //TODO
}

static std::unique_ptr<GenericAST> ParseFunctionCallExpr(){
    //auto callee = std::make_unique<>
}

static std::unique_ptr<GenericAST> ParseVariableNameExpr(){
    //auto value = std::make_unique<V
}

//Binary expression parsing start


//this parses an addition/subtraction expression
//creates an AST data structure with children for each side of the operation
//made up of two MDE with an operator between them
static std::unique_ptr<GenericAST> ParseAddSubExpr(){
    unqiue_ptr<GenericAST> TermAddSub1 = ParseMulDivExpr();
    if (current_token == '+' || current_token == '-'){
        char operator = current_token;
        get_next_token();
        unqiue_ptr<GenericAST> TermAddSub2 = ParseMulDivExpr();
        TermAddSub1 = BinaryExprAST(operator, move(TermAddSub1), move(TermAddSub2));
        return TermAddSub1;
    }
    else {
        return LogErrorPrototype("Expected '+', '-'")
    }
}

//this parses an addition/subtraction expression
//creates an AST data structure with children for each side of the operation
//made up of two terminals with an operator between them
static std::unique_ptr<GenericAST> ParseMulDivExpr(){
    unique_ptr<GenericAST> TermMulDiv1 = ParseTermExpr();
    if (current_token == '/' || current_token == '%' || current_token == '*'){
        char operator = current_token;
        get_next_token();
        unique_ptr<GenericAST> TermMulDiv2 = ParseTermExpr();
        TermMulDiv1 = BinaryExprAST(operator, move(TermMulDiv1), move(TermMulDiv2));
        return TermMulDiv1;
    }
    else{
        return LogErrorPrototype("Expected '/', '%', '*'");
    }
}

//this parses a terminal - int or float
//can be parantheses enclosed
static std::unique_ptr<GenericAST> ParseTermExpr(){
    if (current_token == '('){
        get_next_token();
        unique_ptr<GenericAST> AddSubExpr = ParseAddSubExpr();
        if (current_token == ')'){
            next_symbol();
            return AddSubExpr;
        }
        else{
            return LogErrorPrototype("Expected ')'");
        }
    }
    else if (current_token == token_float_number){
        unique_ptr<GenericAST> FloatNumber = ParseFloatExpr();
        return FloatNumber;
    }
    else if (current_token == token_int_number){
        unique_ptr<GenericAST> IntNumber = ParseIntExpr();
        return IntNumber;
    }
    else {
        return LogErrorPrototype("Expected '(' or numeric symbol");
    }
}

//Binary expression parsing end

//Function definition parsing start

static std::unique_ptr<GenericAST> ParseFunctionPrototype(){
    
}

//Function definition parsing end