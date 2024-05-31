#include "Utils.h"
#include "ErrorPrototype.h"

bool isTypeDeclaration(int token){
    if (token == token_int || token == token_float || token == token_char || token == token_string || token == token_array || token == token_file || token == token_void) {
        return true;
    }
    return false;
}

bool isOperator(int token){
    if (token == '%' || token == '/' || token == '+' || token == '-' || token == '*'){
        return true;
    }
    return false;
}