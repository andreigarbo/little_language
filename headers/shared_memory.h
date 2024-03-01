#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

static double numeric_value_float;
static int numeric_value_int;
static std::string identifier_string;

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

    //strings or numbers
    token_identifier = -15,
    token_int_number = -16
    token_float_number = -17
};

#endif