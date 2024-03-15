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
#include "utils.h"

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

//-----Start of int, float, string parsers

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

//------End of int, float, string parsers

static std::unique_ptr<GenericAST> ParseBinaryExpr(){
    //TODO
}

static std::unique_ptr<GenericAST> ParseVariableAssignExpr(){
    if (current_token == token_identifier){
        auto variable = std::make_unique<StringAST>(identifier_string);
        if (current_token == '=') {
            get_next_token();
            if(current_token == token_identifier){
                auto assigned_value = std::make_unique<StringAST>(identifier_string);
            } else if (current_token == token_int_number){
                auto assigned_value = std::make_unique<IntAST>(numeric_value_int);
            } else if (current_token == token_float_number) {
                auto assigned_value = std::make_unique<FloatAST>(numeric_value_float);
            } else {return LogError("Expected right value of assignment to be string, float or int")}
            auto variableAssignAST = make_unique<VariableAssignAST>(variable, assigned_value);
            return variableAssignAST;
        }
    } else {return LogError("Expected string as variable name");}
}

static std::unique_ptr<GenericAST> ParseVariableNameExpr(){
    //auto value = std::make_unique<V
}

//-----Binary expression parsing start


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

//-----Binary expression parsing end

//-----Conditional statement parsing start

static std::unique_ptr<GenericAST> IfStatementExpr() {
    if (current_token == token_if) {
        get_next_token();
        if (current_token == '('){
            get_next_token();
            auto condition = ParseConditionExpr();
            if (current_token == ')') {
                get_next_token();
                if (current_token == '{'){
                    get_next_token();
                    auto thenBody = ParseStatementsExpr();
                    if (current_token == '}') {
                        get_next_token();
                        if (current_token == token_else) {
                            get_next_token();
                            if (current_token == '{'){
                                get_next_token();
                                auto elseBody = ParseStatementsExpr();
                                if (current_token == '}') {
                                    get_next_token();
                                    unqiue_ptr<GenericAST> ifStatement;
                                    ifStatement = IfASt(std::move(condition), std::move(thenBody), std::move(elseBody));
                                    return ifStatement;
                                } else {return LogError("Expected '}' in end of else block");}
                            } else {return LogError("Expected '{' in beginning of else block");}
                        } else {
                            unique_ptr<GenericAST> ifStatement;
                            ifStatement = IfAST(std::move(condition), std::move(body), nullptr);
                            return ifStatement;
                        }
                    } else {return LogError("Expected '}' in end of then block");}
                } else {return LogError("Expected '{' in beginning of then block");}
            } else {return LogError("Expected ')' in end of condition block");}
        } else {return LogError("Expected '(' in beginning of condition block");}
    } else {return LogError("Expected 'if' in beginning of conditional statement");}
}

static std::unqiue_ptr<GenericAST> WhileStatementExpr(){
    if (current_token == token_while){
        get_next_token();
        if (current_token == '('){
            get_next_token();
            auto condition = ParseConditionExpr();
            if (current_token == ')') {
                get_next_token();
                if (current_token == '{') {
                    get_next_token();
                    auto body = ParseStatementsExpr();
                    if (current_token == '}') {
                        get_next_token();
                        unique_ptr<GenericAST> whileStatement;
                        whileStatement = WhileAST(condition, body);
                        return whileStatement;
                    } else {return LogError("Expected '}' in end of instruction block");}
                } else {return LogError("Expected '{' in beginning of instruction block");}
            } else {return LogError("Expected ')' in end of condition block");}
        } else {return LogError("Expected '(' in beginning of condition block");}
    } else {return LogError("Expected 'while' in beginning of while loop");}
}

static std::unique_ptr<GenericAST> ForStatementExpr() {
    if (current_token == token_for){
        get_next_token();
        if (current_token == '(') {
            get_next_token();
            auto firstArgumentIterator = ParseStringExpr();
            if (current_token == token_in) {
                get_next_token();
                auto iterableName = ParseStringExpr();
                if(current_token == ')') {
                    get_next_token();
                    //fill code to parse body of expression
                    //then return ForIteratorAST (needs to be created as a header file as well)
                    if (current_token == '{') {
                        get_next_token();
                        auto body = ParseStatementsExpr();
                        if (current_token == '}') {
                            get_next_token();
                            unqiue_ptr<GenericAST> forIteratorAST;
                            forIteratorAST = ForInAST(firstArgumentIterator, iterableName, body);
                        } else {return LogError("Expected '}' at end of instruction block");}
                    } else {return LogError("Expected '{' at start of instruction block");}
                } else {return LogError("Expected ')' in end of iterator block");}
            } 
            else if (current_token == token_to) {
                get_next_token();
                auto secondArgumentIterator = ParseStringExpr();
                if (current_token == ',') {
                    get_next_token();
                    if (current_token == token_iterate) {
                        get_next_token();
                        if (current_token == '=') {
                            get_next_token();
                            auto iteratorIncrement = ParseIntExpr();
                            if(current_token == ')') {
                                get_next_token();
                                //fill code to parse body of expression
                                //then return ForRangeAST (needs to be created as a header file as well)
                                if (current_token == '{') {
                                    get_next_token();
                                    auto body = ParseStatementsExpr();
                                    if (current_token == '}') {
                                        get_next_token();
                                        unique_ptr<GenericAST> forRangeAST;
                                        forRangeAST = ForAST(firstArgumentIterator, secondArgumentIterator, iteratorIncrement, body);
                                    }
                                } else {return LogError("Expected '{' at start of instruction block");}
                            } else {return LogError("Expected ')' in end of iterator block");}
                        } else {return LogError("Expected '=' after keyword 'iterate'");}
                    } else {return LogError("Expected argument 'iterate'");}
                } else {
                    if(current_token == ')') {
                        get_next_token();
                    } else {return LogError("Expected ')' in end of iterator block");}
                }
            }
            else {return LogError("Expected keyword 'in' or 'to' after first argument in iterator block");}
        } else {return LogError("Expected '(' in beginning of iterator block");}
    } else {return LogError("Expected 'for' in beginning of for loop");}
}

static std::unqiue_ptr<GenericAST> DoWhileStatementAST(){
    if(current_token == token_do) {
        get_next_token();
        if (current_token == '{') {
            get_next_token();
            auto body = ParseStatementsExpr();
            if (current_token == '}') {
                get_next_token();
                if (current_token == token_while) {
                    get_next_token();
                    if (current_token == '('){
                        get_next_token();
                        auto condition = ParseConditionExpr();
                        if (current_token == ')') {
                            get_next_token();
                            unqiue_ptr<GenericAST> doWhileStatement;
                            doWhileStatement = DoWhileStatementAST(body, condition);
                            return doWhileStatement;
                        } else {return LogError("Expected ')' at end of condition");}
                    } else {return LogError("Expected '(' after 'while'");}
                } else {return LogError("Expected 'while' at end of instruction block");}
            } else {return LogError("Expected '}' in end of instruction block");}
        } else {return LogError("Expected '{' in beginning of instruction block");}
    }
}




//-----Conditional statement parsing end


//-----Function definition parsing start

//this function parses the prototype of a function
//e.g. int main (int arg, float val, string test)

//TODO - modify function to generate FunctionPrototypeArgumentAST for each argument

static std::unique_ptr<GenericAST> ParseFunctionPrototypeExpr(){
    //checks if the first symbol is a type declaration  
    if (isTypeDeclaration(current_token)){
        //parses a string, which will be the type declaration
        auto functionType = ParseStringExpr();
        try{
            //parses a string, which will be the name of the function
            auto functionName = ParseStringExpr();
        } catch () {
            //handles incorrect string
            return LogError("Expected function name to be a string");
        }
        if (current_token == '('){
            get_next_token();
            //parsing arguments one by one
            if (isTypeDeclaration(current_token)){
                std::vector<GenericAST> argumentList;
                auto argument = ParseFunctionPrototypeArgumentExpr();
                argumentList.push_back(argument);
                while (current_token == ','){
                    get_next_token();
                    auto argument = ParseFunctionPrototypeArgumentExpr();
                    argumentList.push_back(argument);
                }
                if (current_token == ')'){
                    get_next_token();
                    try {
                        auto functionProtoype = FunctionPrototypeAST(functionType, functionName, argumentList);//generate Prototype AST and return
                    } catch() {
                        return LogError("Incorrectly formatted function prototype"); // handles errors in creation of AST node
                    }
                } else {return LogError("Expected ')' at end of function prototype");} // handles lack of ')' on prototype arg list end
            } else {return LogError("Expected argument type before name");}

        } else {return LogError("Expected '('");} //handles lack of '(' after function name
    } else {return LogError("Expected function type at beginning of declaration");} //handles no string at start of line
}

static std::unique_ptr<GenericAST> ParseFunctionPrototypeArgumentExpr() {
    std::string argumentType = ParseStringExpr();
    std::string argumentName = ParseStringExpr();
    if (current_token == '='){
        get_next_token();
        switch (argumentType) {
            case "int":
                auto value = ParseIntExpr();
                auto argument = std::make_unique<FunctionPrototypeArgumentIntAST>(argumentName, value);
                return argument;
                break;
            case "float":
                auto value = ParseFloatExpr();
                auto argument = std::make_unique<FunctionPrototypeArgumentFloatAST>(argumentName, value);
                return argument;
                break;
            case "string":
                auto value = ParseStringExpr();
                auto argument = std::make_unique<FunctionPrototypeArgumentStringAST>(argumentName, value);
                return argument;
                break;
            case "default":
                return LogError("Unexpected argument type for argument " + argumentName);
                break;
        }
    } else {
        switch(argumentType){
            case "int":
                auto argument = std::make_unique<FunctionPrototypeArgumentIntAST>(argumentName, nullptr);
                return argument;
                break;
            case "float":
                auto argument = std::make_unique<FunctionPrototypeArgumentFloatAST>(argumentName, nullptr);
                return argument;
                break;
            case "string":
                auto argument = std::make_unique<FunctionPrototypeArgumentStringAST>(argumentName, nullptr);
                return argument;
                break;
            case default:
                return LogError("Unexpected argument type for argument " + argumentName);
                break;
        }
    }
}

static std::unique_ptr<GenericAST> ParseFunctionCallExpr() {
    auto functionName = ParseStringExpr();
    if (current_token == '('){
        get_next_token();
        std::vector<GenericAST> functionArgs;
        auto functionArg = ParseFunctionArgumentExpr();
        functionArgs.push_back(functionArg);
        while (current_token == ','){
            get_next_token();
            auto functionArg = ParseFunctionArgumentExpr();
            functionArgs.push_back(functionArg);      
        }
        if (current_token == ')') {
            get_next_token();
            return FunctionCallAST(functionName, functionArgs);
        } else {return LogError("Expected ')' at the end of arguments");}
    } else {return LogError("Expected '(' after function name");}
}

static std::unique_ptr<GenericAST> ParseFunctionArgumentExpr(){
    switch (current_token) {
        case token_int_number:
            auto value = ParseIntExpr();
            return value;
            break;
        case token_float_number:
            auto value = ParseFloatExpr();
            return value;
            break;
        case token_identifier:
            auto value = ParseStringExpr();
            return value;
            break;
        case default:
            return LogError("Unexepected type for function argument");
    }
}

//-----Function definition parsing end

//-----Statement parsing start

//-----Statement parsing end