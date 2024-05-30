#ifndef LEXER_H
#define LEXER_H

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include "Token.h"

class Lexer {
private:
    int last_char;
    int get_token();
    std::ifstream file;
public:
    Lexer() : last_char(' ') {}
    virtual ~Lexer() = default;
    void setFilename(const std::string& filename);
    int get_next_token_from_input();
    double numeric_value_float;
    int numeric_value_int;
    std::string identifier_string;
};

#endif