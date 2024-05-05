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
    token_from = -15,
    token_to = -16,
    token_iterate = -17,

    //strings or numbers
    token_identifier = -18,
    token_int_number = -19,
    token_float_number = -20,

    //operators
    token_le = -21,
    token_ge = -22,
    token_eq = -23,
    token_ne = -24
};

#endif