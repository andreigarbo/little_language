#include "Parser.h"
#include "ErrorPrototype.h"

void Parser::get_next_token(){
    current_token = lexer.get_next_token_from_input();
    numeric_value_float = lexer.numeric_value_float;
    numeric_value_int = lexer.numeric_value_int;
    identifier_string = lexer.identifier_string;
}

std::unique_ptr<IntAST> Parser::ParseIntExpr(){
    if (current_token == token_int_number){
        auto value = std::make_unique<IntAST>(numeric_value_int);
        get_next_token();
        return value;
    } else {return LogErrorPrototypeInt("Expected integer value");}
}

std::unique_ptr<FloatAST> Parser::ParseFloatExpr(){
    if (current_token == token_float_number) {
        auto value = std::make_unique<FloatAST>(numeric_value_float);
        get_next_token();
        return value;
    } else {return LogErrorPrototypeFloat("Expected floating point value");}
}

// static std::unique_ptr<StringAST> ParseStringExpr(){
//     if (current_token == token_identifier && (identifier_string.length() > 1 || identifier_string.length() == 0)){
//         auto value = std::make_unique<StringAST>(identifier_string);
//         get_next_token();
//         return value;
//     } else {return LogErrorPrototypeString("Expected string value");}
// }

std::unique_ptr<StringAST> Parser::ParseStringExpr(){
    if (current_token == token_string_value){
        auto value = std::make_unique<StringAST>(identifier_string);
        get_next_token();
        return value;
    } else {return LogErrorPrototypeString("Expected string value");}
}

std::unique_ptr<CharAST> Parser::ParseCharExpr(){
    if (current_token == token_identifier && identifier_string.length() == 1){
        auto value = std::make_unique<CharAST>(identifier_string.c_str()[0]);
        get_next_token();
        return value;
    } else {return LogErrorPrototypeChar("Expected char value");}
}

std::unique_ptr<ArrayAST> Parser::ParseArrayExpr(){
    if (current_token == '['){
        get_next_token();
        std::vector<std::unique_ptr<GenericAST>> arrayVector;
        //if token not ], expecting at least one argument
        if (current_token != ']') {
            auto currentTerm = ParseTermExpr();
            arrayVector.push_back(std::move(currentTerm));
            while (current_token == ','){
                get_next_token();
                auto currentTerm = ParseTermExpr();
                arrayVector.push_back(std::move(currentTerm));
            }
        }
        if (current_token == ']'){
            get_next_token();
            auto array = std::make_unique<ArrayAST>(std::move(arrayVector));
            return array;
        } else {return LogErrorPrototypeArray("Expected array to end with ']'");}
    } else {return LogErrorPrototypeArray("Expected array to start with '['");}
}

//------End of int, float, string parsers

//-----Binary expression parsing start

//this parses a terminal - int or float
//can be parantheses enclosed

std::unique_ptr<GenericAST> Parser::ParseTermExpr(){
    if (current_token == '('){
        get_next_token();
        std::unique_ptr<GenericAST> AddSubExpr = ParseAddSubExpr();
        if (current_token == ')'){
            get_next_token();
            return AddSubExpr;
        }
        else{
            // std::cout << numeric_value_int<<std::endl;
            return LogErrorPrototype("Expected ')'");
        }
    }
    else if (current_token == token_float_number){
        std::unique_ptr<FloatAST> FloatNumber = ParseFloatExpr();
        return FloatNumber;
    }
    else if (current_token == token_int_number){
        std::unique_ptr<IntAST> IntNumber = ParseIntExpr();
        return IntNumber;
    }
    // else if (current_token == token_identifier && identifier_string.length() == 1){
    //     auto variableName = ParseCharExpr();
    //     return variableName;
    // }
    // else if (current_token == token_identifier && (identifier_string.length() > 1 || identifier_string.length() == 0)){
    //     auto variableName = ParseStringExpr();
    //     return variableName;
    // }
    else if (current_token == token_identifier){
        std::string variableName = identifier_string;
        // auto variableNameAST = std::make_unique<StringAST>(variableName);
        get_next_token();
        if (current_token == '('){
            get_next_token();
            std::vector<std::unique_ptr<GenericAST>> argumentList;
            //parsing arguments one by one
            if(current_token != ')'){
                auto argument = ParseTermExpr();
                argumentList.push_back(std::move(argument));
                while(current_token == ','){
                    get_next_token();
                    auto argument = ParseTermExpr();
                    argumentList.push_back(std::move(argument));
                }
                if (current_token == ')'){
                    get_next_token();
                    auto functionCall = std::make_unique<FunctionCallAST>(std::move(variableName), std::move(argumentList));
                    return functionCall;
                }
            } else if (current_token == ')'){
                get_next_token();
                auto functionCall = std::make_unique<FunctionCallAST>(std::move(variableName), std::move(argumentList));
                return functionCall;
            } else {return LogErrorPrototypeFunctionPrototype("Expected ')' in end of function call");}
        //meaning we have something like x = array[5];
        } else if (current_token == '[') {
            get_next_token();
            auto array_index = ParseTermExpr();
            if (current_token == ']') {
                get_next_token();
                auto arrayElementAccess = std::make_unique<ArrayAccessAST>(std::move(variableName), std::move(array_index), false, nullptr);
                return arrayElementAccess;
            } else {return LogErrorPrototype("Expected ']' after array index");}
        } else {
            return std::make_unique<VariableAST>(variableName, 0, nullptr);
            // return variableNameAST;
        }
    }
    else if (current_token == '['){
        auto array = ParseArrayExpr();
        return array;
    }
    else if (current_token == token_string_value) {
        auto string_value = ParseStringExpr();
        return string_value;
    }
    else {
        return LogErrorPrototype("Expected '(' or numeric symbol when parsing a term");
    }
}

//this parses an addition/subtraction expression
//creates an AST data structure with children for each side of the operation
//made up of two terminals with an operator between them
std::unique_ptr<GenericAST> Parser::ParseMulDivExpr(){
    std::unique_ptr<GenericAST> TermMulDiv1 = ParseTermExpr();
    while (current_token == '/' || current_token == '%' || current_token == '*'){
        char op = current_token;
        get_next_token();
        std::unique_ptr<GenericAST> TermMulDiv2 = ParseTermExpr();
        TermMulDiv1 = std::make_unique<BinaryExprAST>(op, std::move(TermMulDiv1), std::move(TermMulDiv2));
    }
    return TermMulDiv1; 
}

//this parses an addition/subtraction expression
//creates an AST data structure with children for each side of the operation
//made up of two MDE with an operator between them
std::unique_ptr<GenericAST> Parser::ParseAddSubExpr(){
    std::unique_ptr<GenericAST> TermAddSub1 = ParseMulDivExpr();
    while (current_token == '+' || current_token == '-' || (current_token == token_int_number && numeric_value_int < 0) || (current_token == token_float_number && numeric_value_float < 0.0)){
        // std::cout << current_token <<std::endl;
        char op = current_token;
        get_next_token();
        std::unique_ptr<GenericAST> TermAddSub2 = ParseMulDivExpr();
        TermAddSub1 = std::make_unique<BinaryExprAST>(op, std::move(TermAddSub1), std::move(TermAddSub2));
    }
    return TermAddSub1;
}

//-----Binary expression parsing end

//-----Conditional statement parsing start

std::unique_ptr<IfAST> Parser::ParseIfStatementExpr() {
    if (current_token == token_if) {
        get_next_token();
        if (current_token == '('){
            get_next_token();
            auto condition = ParseConditionExpr();
            if (current_token == ')') {
                get_next_token();
                auto thenBody = ParseFunctionBodyExpr();
                if (current_token == token_else) {
                    get_next_token();
                    auto elseBody = ParseFunctionBodyExpr();
                    std::unique_ptr<IfAST> ifStatement;
                    ifStatement = std::make_unique<IfAST>(std::move(condition), std::move(thenBody), std::move(elseBody));
                    return ifStatement;
                } else {
                    std::unique_ptr<IfAST> ifStatement;
                    ifStatement = std::make_unique<IfAST>(std::move(condition), std::move(thenBody));
                    return ifStatement;
                }
            } else {return LogErrorPrototypeIf("Expected ')' in end of condition block");}
        } else {return LogErrorPrototypeIf("Expected '(' in beginning of condition block");}
    } else {return LogErrorPrototypeIf("Expected 'if' in beginning of conditional statement");}
}

std::unique_ptr<WhileAST> Parser::ParseWhileStatementExpr(){
    if (current_token == token_while){
        get_next_token();
        if (current_token == '('){
            get_next_token();
            auto condition = ParseConditionExpr();
            if (current_token == ')') {
                get_next_token();
                auto body = ParseFunctionBodyExpr();
                std::unique_ptr<WhileAST> whileStatement;
                whileStatement = std::make_unique<WhileAST>(std::move(condition), std::move(body));
                return whileStatement;
            } else {return LogErrorPrototypeWhile("Expected ')' in end of condition block");}
        } else {return LogErrorPrototypeWhile("Expected '(' in beginning of condition block");}
    } else {return LogErrorPrototypeWhile("Expected 'while' in beginning of while loop");}
}

std::unique_ptr<DoWhileAST> Parser::ParseDoWhileStatementExpr(){
    if(current_token == token_do) {
        get_next_token();
        auto body = ParseFunctionBodyExpr();
        if (current_token == token_while) {
            get_next_token();
            if (current_token == '('){
                get_next_token();
                auto condition = ParseConditionExpr();
                if (current_token == ')') {
                    get_next_token();
                    if (current_token == ';') {
                        get_next_token();
                    } else {return LogErrorPrototypeDoWhile("Expected ';' after do...while statement");}
                    std::unique_ptr<DoWhileAST> doWhileStatement;
                    doWhileStatement = std::make_unique<DoWhileAST>(std::move(condition), std::move(body));
                    return doWhileStatement;
                } else {return LogErrorPrototypeDoWhile("Expected ')' at end of condition");}
            } else {return LogErrorPrototypeDoWhile("Expected '(' after 'while'");}
        } else {return LogErrorPrototypeDoWhile("Expected 'while' at end of instruction block");}
    }
}

std::unique_ptr<GenericAST> Parser::ParseForStatementExpr() {
    if (current_token == token_for){
        get_next_token();
        if (current_token == '(') {
            get_next_token();
            std::string variableName = identifier_string;
            get_next_token();
            if (current_token == token_in) {
                std::unique_ptr<GenericAST> iterable;
                get_next_token();
                if (current_token == '[') {
                    iterable = ParseArrayExpr(); // needed because maybe we have (for var in [1,2,3])
                } else {
                    iterable = ParseTermExpr();
                }
                if(current_token == ')') {
                    get_next_token();
                    auto body = ParseFunctionBodyExpr();
                    auto forIteratorAST = std::make_unique<ForInAST>(std::move(variableName), std::move(iterable), std::move(body));
                    return forIteratorAST;
                } else {return LogErrorPrototype("Expected ')' in end of iterator block");}
            } 
            else if (current_token == token_from) {
                get_next_token();
                if (current_token != token_int_number && current_token != token_identifier) {
                    return LogErrorPrototype("Expected integer value as for range start");
                }
                auto fromValue = ParseTermExpr();
                if (current_token == token_to){
                    get_next_token();
                    if (current_token != token_int_number && current_token != token_identifier){
                        return LogErrorPrototype("Expected integer value as for range start");
                    }
                    auto toValue = ParseTermExpr();
                    int iterate = 1;
                    if (current_token == ','){
                        get_next_token();
                        if (current_token == token_iterate){
                            get_next_token();
                            if (current_token == '='){
                                get_next_token();
                                if (current_token == token_int_number){
                                    iterate = numeric_value_int;
                                    get_next_token();
                                } else {return LogErrorPrototype("Expected integer value for 'iterate' parameter");}
                            } else {return LogErrorPrototype("Expected '=' after 'iterate' parameter");}
                        } else {return LogErrorPrototype("Valid parameters for 'for' loop are: iterate");}
                    }
                    if (current_token == ')'){
                        get_next_token();
                        auto body = ParseFunctionBodyExpr();
                        auto forRangeAST = std::make_unique<ForRangeAST>(std::move(variableName), std::move(fromValue), std::move(toValue), std::move(iterate), std::move(body)); 
                        return forRangeAST;
                    } else {return LogErrorPrototype("Expected ')' afer for loop definition");}
                } else {return LogErrorPrototype("Expected 'to' keyword in for loop declaration");}
            } else {return LogErrorPrototype("Expected keyword 'in' or 'from' after first argument in iterator block");}
        } else {return LogErrorPrototype("Expected '(' in beginning of iterator block");}
    } else {return LogErrorPrototype("Expected 'for' in beginning of for loop");}
}

std::unique_ptr<GenericAST> Parser::ParseConditionExpr(){
    bool negate_left_side = false;
    if (current_token == '!') {
        negate_left_side = true;
        get_next_token();
    }
    auto left_side = ParseTermExpr();
    if (current_token == ')') {
        // if (negate_left_side) {
        //     return std::make_unique<BinaryExprAST>(token_eq, std::move(left_side), nullptr);
        // } else {
        //     return std::make_unique<BinaryExprAST>(token_ne, std::move(left_side), nullptr);
        // }
        return LogErrorPrototype("Expected binary expression as condition");
    }
    if (current_token == '<' || current_token == '>' || current_token == token_le || current_token == token_ge || current_token == token_eq || current_token == token_ne) {
        char op = current_token;
        get_next_token();
        auto right_side = ParseTermExpr();
        return std::make_unique<BinaryExprAST>(op, std::move(left_side), std::move(right_side));
    } else {
        return left_side;
    }
}

// Conditional statement parsing end

std::unique_ptr<GenericAST> Parser::ParseGlobalVariableExpr(){
    if (current_token == token_global){
        get_next_token();
        auto declarationOrAssignment = ParseDeclarationOrAssignmentExpr();
        if (current_token == ';'){
            get_next_token();
            return declarationOrAssignment;
        } else {return LogErrorPrototype("Expected ';' after global statement");}
    }
}

std::unique_ptr<GenericAST> Parser::ParseDeclarationOrAssignmentExpr(){
    if (current_token == token_identifier || isTypeDeclaration(current_token)){
        //signals a variable declaration
        //first token is int, float, string, void, etc...
        if (isTypeDeclaration(current_token)){
            //keep type as an int value of token, can be referenced from the AST file when doing codegen in lexer_parser_commons
            int type = current_token;
            get_next_token();
            //keep name of variable or function as string
            std::string name = identifier_string;
            get_next_token();
            //meaning that this variable declaration includes an assignment
            if (current_token == '='){
                get_next_token();
                std::unique_ptr<GenericAST> rightSide = ParseTermExpr();
                if (current_token != ';') {
                    return LogErrorPrototype("Expected ';' after variable declaration");
                }
                get_next_token();
                std::unique_ptr<GenericAST> variableDeclaration;
                variableDeclaration = std::make_unique<VariableAST>(name, type, std::move(rightSide));
                return variableDeclaration;
            }
            //meaning that this is a simple variable declaration, without an assignment
            else if (current_token == '(') {
                // std::unique_ptr<FunctionAST> Parser::ParseFunctionExpr(){
                //     auto prototype = ParseFunctionPrototypeExpr();
                //     if (current_token == ';'){
                //         get_next_token();
                //         //meaning that this is a prototype declaration only
                //         std::vector<std::unique_ptr<GenericAST>> emptyBody;
                //         auto function = std::make_unique<FunctionAST>(std::move(prototype), std::move(emptyBody));
                //         return function;
                //     }
                //     auto body = ParseFunctionBodyExpr();
                //     auto function = std::make_unique<FunctionAST>(std::move(prototype), std::move(body));
                //     return function;
                // }
                std::unique_ptr<GenericAST> function = ParseFunctionExpr(type, name);
                return function;

            }
            else{
                std::string name = identifier_string;
                if (current_token != ';') {
                    return LogErrorPrototype("Expected ';' after variable declaration");
                }
                get_next_token();
                std::unique_ptr<GenericAST> variableDeclaration;
                variableDeclaration = std::make_unique<VariableAST>(name, type, nullptr);
                return variableDeclaration;
            }
        }
        //a regular string, signals variable assignment like a=5 or a function call function(a,b,c)
        else {
            std::string name = identifier_string;
            get_next_token();
            if (current_token == '[') {
                get_next_token();
                auto array_index = ParseTermExpr();
                if (current_token == ']') {
                    get_next_token();
                    if (current_token == '=') {
                        get_next_token();
                        auto newValue = ParseTermExpr();
                        get_next_token();
                        auto arrayElementAccess = std::make_unique<ArrayAccessAST>(std::move(name), std::move(array_index), true, std::move(newValue));
                        return arrayElementAccess;
                    } else {return LogErrorPrototype("Expected rvalue in array cell assignment");}
                } else {return LogErrorPrototype("Expected ']' after array index");}
            }
            else if (current_token == '='){
                get_next_token();
                std::unique_ptr<GenericAST> rightSide = ParseTermExpr();
                if (current_token != ';') {
                    return LogErrorPrototype("Expected ';' after variable assignment");
                }
                get_next_token();
                std::unique_ptr<GenericAST> variableDeclaration;
                variableDeclaration = std::make_unique<VariableAST>(name, 0, std::move(rightSide));
                return variableDeclaration;
            } else if (current_token == '('){
                get_next_token();
                std::vector<std::unique_ptr<GenericAST>> argumentList;
                //parsing arguments one by one
                if(current_token != ')'){
                    auto argument = ParseTermExpr();
                    argumentList.push_back(std::move(argument));
                    while(current_token == ','){
                        get_next_token();
                        auto argument = ParseTermExpr();
                        argumentList.push_back(std::move(argument));
                    }
                    if (current_token == ')'){
                        get_next_token();
                        if (current_token != ';') {
                            return LogErrorPrototype("Expected ';' after variable assignment");
                        }
                        get_next_token();
                        auto functionCall = std::make_unique<FunctionCallAST>(std::move(name), std::move(argumentList));
                        return functionCall;
                    }
                } else if (current_token == ')'){
                    get_next_token();
                    if (current_token != ';') {
                        return LogErrorPrototype("Expected ';' after variable assignment");
                    }
                    get_next_token();
                    auto functionCall = std::make_unique<FunctionCallAST>(std::move(name), std::move(argumentList));
                    return functionCall;
                } else {return LogErrorPrototypeFunctionPrototype("Expected ')' in end of function call");}
            } else {return LogErrorPrototype("Expected '=' after variable name");}
        }
    } else {return LogErrorPrototype("Expected identifier at start of declaration or assignment");}
}

std::unique_ptr<GenericAST> Parser::ParseFunctionPrototypeArgumentExpr() {
    //first token is the type declaration
    if (isTypeDeclaration(current_token)){
        std::string argumentType = identifier_string;
        get_next_token();
        //second token is the name of the argument
        if (current_token == token_identifier){
            std::string argumentName = identifier_string;
            get_next_token();
            //if token is '=', argument has a default value
            if (current_token == '='){
                get_next_token();
                if (argumentType == "int"){
                    auto value = ParseIntExpr();
                    auto argument = std::make_unique<FunctionPrototypeArgumentIntAST>(argumentName, std::move(value));
                    return argument;
                }
                else if (argumentType== "float"){
                    auto value = ParseFloatExpr();
                    auto argument = std::make_unique<FunctionPrototypeArgumentFloatAST>(argumentName, std::move(value));
                    return argument;
                }
                else if (argumentType == "string"){
                    auto value = ParseStringExpr();
                    auto argument = std::make_unique<FunctionPrototypeArgumentStringAST>(argumentName, std::move(value));
                    return argument;
                }
                else if (argumentType == "char"){
                    auto value = ParseCharExpr();
                    auto argument = std::make_unique<FunctionPrototypeArgumentCharAST>(argumentName, std::move(value));
                    return argument; 
                }
                else if (argumentType == "array"){
                    auto value = ParseArrayExpr();
                    // auto argument = std::make_unique<FunctionPrototypeArgumentArrayAST>(argumentName, std::move(value));
                    // return argument;
                }
                else{
                    // std::cout << argumentType << std::endl;
                    return LogErrorPrototype("Unexpected argument type");
                }
            } else {
                std::string arrayType = "none";
                int arraySize = -1;
                //paranthesis for array parameters
                if (current_token == '(') {
                    get_next_token();
                    if(isTypeDeclaration(current_token)){
                        arrayType = identifier_string;
                        get_next_token();
                        if (current_token == ','){
                            get_next_token();
                            if (current_token == token_int_number) {
                                arraySize = numeric_value_int;
                                get_next_token();
                                if (current_token == ')') {
                                    get_next_token();
                                } else {return LogErrorPrototype("Expected ')' after parameters for array parameter");}
                            } else {return LogErrorPrototype("Expected integer value for array parameter size");}
                        } else {return LogErrorPrototype("Expected ',' after type declaration for array parameter");}
                    } else {return LogErrorPrototype("Expected type as first argument of array parameter");}
                }
                if (argumentType == "int"){
                    auto argument = std::make_unique<FunctionPrototypeArgumentIntAST>(argumentName, nullptr);
                    return argument;
                }
                else if (argumentType == "float"){
                    auto argument = std::make_unique<FunctionPrototypeArgumentFloatAST>(argumentName, nullptr);
                    return argument;
                }
                else if (argumentType == "string"){
                    auto argument = std::make_unique<FunctionPrototypeArgumentStringAST>(argumentName, nullptr);
                    return argument;
                }
                else if (argumentType == "char") {
                    auto argument = std::make_unique<FunctionPrototypeArgumentCharAST>(argumentName, nullptr);
                    return argument;
                }
                else if (argumentType == "array") {
                    // std::cout << arrayType << " "  << arraySize << std::endl;
                    if (arrayType == "none" || arraySize == -1) {
                        return LogErrorPrototype("Invalid or no parameters provided for array argument");
                    }
                    auto argument = std::make_unique<FunctionPrototypeArgumentArrayAST>(argumentName, nullptr, arrayType, arraySize);
                    return argument;
                }
                else {
                    return LogErrorPrototype("Unexpected argument type");
                }
            }
        }
    }
}

std::unique_ptr<FunctionPrototypeAST> Parser::ParseFunctionPrototypeExpr(int functionType, std::string functionName){
    if (current_token == '('){
        get_next_token();
        std::vector<std::unique_ptr<GenericAST>> argumentList;
        //parsing arguments one by one
        if (isTypeDeclaration(current_token)){
            auto argument = ParseFunctionPrototypeArgumentExpr();
            argumentList.push_back(std::move(argument));
            while (current_token == ','){
                get_next_token();
                auto argument = ParseFunctionPrototypeArgumentExpr();
                argumentList.push_back(std::move(argument));
            } 
        }
        if (current_token == ')'){
            get_next_token();   
            auto functionProtoype = std::make_unique<FunctionPrototypeAST>(functionType, functionName, std::move(argumentList));//generate Prototype AST and return
            return functionProtoype;
        } else {return LogErrorPrototypeFunctionPrototype("Expected ')' in end of function prototype");}
    } else {return LogErrorPrototypeFunctionPrototype("Expected '(' in beginning of function prototype");} //handles lack of '(' after function name
}

std::unique_ptr<GenericAST> Parser::ParseStatementExpr(){
    if (current_token == token_if){
        // std::cout << "Detected 'if'. Parsing if statement" << std::endl;
        return ParseIfStatementExpr();
    }
    else if (current_token == token_while){
        // std::cout << "Detected 'while'. Parsing while statement" << std::endl;
        return ParseWhileStatementExpr();
    }
    else if (current_token == token_do) {
        // std::cout << "Detected 'do...while'. Parsing do...while statement" << std::endl;
        return ParseDoWhileStatementExpr();
    }
    else if (current_token == token_for) {
        return ParseForStatementExpr();
    }
    else if (current_token == token_identifier || isTypeDeclaration(current_token)){
        // std::cout << "Detected identifier or type declaration. Parsing declaration or assignment" << std::endl;
        auto declarationOrAssignment = ParseDeclarationOrAssignmentExpr();
        return declarationOrAssignment;

        // if (current_token == ';'){
        //     get_next_token();
        //     return declarationOrAssignment;
        // } else {return LogErrorPrototype("Expected ';' after statement");}
    }
    else if (current_token == token_return){
        // std::cout << "Detected return statement\n";
        auto returnStatement = ParseReturnExpr();
        if (current_token == ';'){
            get_next_token();
            return returnStatement;
        } else {return LogErrorPrototypeReturn("Expected ';' after return statement");}
    }
}

std::unique_ptr<FunctionAST> Parser::ParseFunctionExpr(int functionType, std::string functionName){
    auto prototype = ParseFunctionPrototypeExpr(functionType, functionName);
    if (current_token == ';'){
        get_next_token();
        //meaning that this is a prototype declaration only
        std::vector<std::unique_ptr<GenericAST>> emptyBody;
        auto function = std::make_unique<FunctionAST>(std::move(prototype), std::move(emptyBody));
        return function;
    }
    auto body = ParseFunctionBodyExpr();
    auto function = std::make_unique<FunctionAST>(std::move(prototype), std::move(body));
    return function;
}

std::vector<std::unique_ptr<GenericAST>> Parser::ParseFunctionBodyExpr(){
    if (current_token == '{'){
        std::vector<std::unique_ptr<GenericAST>> functionBody;
        get_next_token();
        do {
            std::unique_ptr<GenericAST> statement;
            statement = ParseStatementExpr();
            functionBody.push_back(std::move(statement));
            if (current_token == token_eof){
                return LogErrorPrototypeVector("Unexpected end of file while parsing function");
            }
        } while (current_token != '}');
        if (current_token == '}'){
            get_next_token();
            return functionBody;
        } else {return LogErrorPrototypeVector("Expected function to terminate with '}'");}
    } else {return LogErrorPrototypeVector("Expected '{' in beginning of function body");}
}

std::unique_ptr<ImportAST> Parser::ParseImportExpr(){
    if (current_token == token_import){
        get_next_token();
        if (current_token == token_identifier){
            std::string libraryName = identifier_string;
            get_next_token();
            return std::make_unique<ImportAST>(std::move(libraryName));
        } else {return LogErrorPrototypeImport("Expected string after 'import' keyword");}
    }
}

std::unique_ptr<ReturnAST> Parser::ParseReturnExpr(){
    if (current_token == token_return){
        get_next_token();
        if (current_token == ';'){
            return std::make_unique<ReturnAST>(nullptr);
        }
        auto returnValue = ParseTermExpr();
        return std::make_unique<ReturnAST>(std::move(returnValue));
    }
}

std::vector<std::unique_ptr<GenericAST>> Parser::ParseFile(){
    lexer.setFilename(filename);
    get_next_token();
    while(true){
        switch(current_token){
            case token_eof:
            {
                // std::cout << "EOF detected" << std::endl;
                return std::move(ASTarray);
            }
            case token_identifier:
            case token_int:
            case token_float:
            case token_char:
            case token_string:
            case token_array:
            case token_void:
            {
                // std::cout << "Detected type definition. Parsing a declaration or assignment" << std::endl;
                // auto function = ParseFunctionExpr();
                auto astNode = ParseDeclarationOrAssignmentExpr();
                ASTarray.push_back(std::move(astNode));
                break;
            }
            case token_global:
            {
                // std::cout << "Detected global variable declaration\n";
                auto globalVariable = ParseGlobalVariableExpr();
                ASTarray.push_back(std::move(globalVariable));
                break;
            }
            case token_import:
            {
                // std::cout << "Detected import statement\n";
                auto importStatement = ParseImportExpr();
                ASTarray.push_back(std::move(importStatement));
                break;
            }
            default:
            {
                // std::cout<<"Expected to find EOF of literal token. Found " << current_token << " or " << (char)current_token << std::endl;
                // std::cout << identifier_string << std::endl;
                return LogErrorPrototypeVector("Unexpected token encountered while parsing");
            }
        }
    }
}
