#ifndef LEXER_PARSER_COMMONS_H
#define LEXER_PARSER_COMMONS_H

//static double numeric_value_float;
//static int numeric_value_int;
//static std::string identifier_string;

enum Token{
    //end of file
    token_eof = -1,
    
    //type declarations
    token_int = -2,
    token_float = -3,
    token_char = -4,
    token_string = -5,
    token_array = -6,
    token_file = -7,
    token_struct = -8,

    //flow control
    token_if = -9,
    token_else = -10,
    token_while = -11,
    token_do = -12,
    token_for = -13,
    token_in = -14,
    token_to = -15,
    token_iterate = -16,

    //strings or numbers
    token_identifier = -17,
    token_int_number = -18,
    token_float_number = -19,

    //operators
    token_le = -20,
    token_ge = -21,
    token_eq = -22,
    token_ne = -23
};

#endif