#ifndef TOKEN_H
#define TOKEN_H

enum Token{
    //end of file
    token_eof = -1,
    
    //type declarations
    token_int = -2,
    token_float = -3,
    token_char = -4,
    token_string = -5,
    token_void = -6,
    token_array = -7,
    token_file = -8,
    token_struct = -9,

    //flow control
    token_if = -10,
    token_else = -11,
    token_while = -12,
    token_do = -13,
    token_for = -14,
    token_in = -15,
    token_from = -16,
    token_to = -17,
    token_iterate = -18,

    //strings or numbers
    token_identifier = -19,
    token_int_number = -20,
    token_float_number = -21,

    //operators
    token_le = -22,
    token_ge = -23,
    token_eq = -24,
    token_ne = -25,

    //misc
    token_global = -26,
    token_import = -27,
    token_return = -28
};

#endif