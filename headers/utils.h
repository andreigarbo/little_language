#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <string>
#include "lexer_parser_commons.h"

bool isTypeDeclaration(int token){
    if (token == token_int || token == token_float || token == token_char || token == token_string || token == token_array || token == token_file){
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

#endif