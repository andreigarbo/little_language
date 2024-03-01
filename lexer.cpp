#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "headers/shared_memory.h"

static std::string identifier_string; // Filled in if tok_identifier
static double numeric_value;

//function for getting tokens from the input
static int get_token(){
    //initialize char to whitespace
    static int last_char = ' ';

    //skips any and all whitespace characters present before the token
    while(isspace(last_char)){
        last_char = getchar();
    }

    //characters in range [a-zA-Z]*
    if (isalpha(last_char)){
        identifier_string = last_char;

        //while characters read are in the above mentioned range
        while(isalpha(last_char = getchar())){

            //build string from the read characters
            identifier_string += last_char;
        }

        //return the found tokens
        if(identifier_string == "int"){
            return token_int;
        }
        else if(identifier_string == "float"){
            return token_float;
        }
        else if(identifier_string == "char"){
            return token_char;
        }
        else if(identifier_string == "string"){
            return token_string;
        }
        else if(identifier_string == "array"){
            return token_array;
        }
        else if(identifier_string == "file"){
            return token_file;
        }
        else if(identifier_string == "struct"){
            return token_struct;
        }
        else if(identifier_string == "if"){
            return token_if;
        }
        else if(identifier_string == "else"){
            return token_else;
        }
        else if(identifier_string == "while"){
            return token_while;
        }
        else if(identifier_string == "do"){
            return token_do;
        }
        else if(identifier_string == "for"){
            return token_for;
        }
        else if(identifier_string == "in"){
            return token_in;
        }

        //if not a specially defined keyword, return token_identifier to signal a custom word (such as a variable name)
        
        return token_identifier; 
    }

    //for int numbers or float numbers, format 1.2345.....
    else if (isdigit(last_char) || last_char == '.'){ 
        //variable that will store the characters as they are read
        std::string read_number;

        //this variable is to make sure that we don't read multiple decimal points (example: 1.23.45)
        bool has_read_decimal_point = false;

        //read characters and build string
        do{
            //this case is for the first decimal point to appear, this is kept
            if(last_char == '.' && has_read_decimal_point == false){
                has_read_decimal_point = true;
                read_number += last_char;
                last_char = getchar();
            }
            //this skips any further decimal points, making 1.23.45 equal to 1.2345
            else if(last_char =='.' && has_read_decimal_point == true){
                last_char = getchar();
            }
            //this case is for numerical characters
            else{
                read_number += last_char;
                last_char = getchar();
            }
        } while (isdigit(last_char) || last_char == '.');

        //strtod takes 2 arguments, first the value to be converted to a floating point number
        //and then a pointer where it can store any characters that are not numeric (nullptr should discard any non numeric characters)
        //string.c_str() converts a string to a null terminated character array
        if !has_read_decimal_point{
            numeric_value_int = strtol(read_number.c_str(), nullptr, 10);
            return token_int_number;
        } else {
            numeric_value_float = strtod(read_number.c_str(), nullptr)
            return token_float_number;
        }
    }
    //single line comments start with #
    //multi line comments start and end with ##
    else if (last_char == "#"){
        last_char = getchar();
        //multi line comment
        if(last_char == '#'){
            do{
                last_char = getchar();
            } while(last_char != '#');
            last_char = getchar();
            if(last_char == '#'){ // correctly formatted multi line comment
                last_char = getchar();
                if(last_char != EOF){
                    return get_token(); //skips the whole comment as a single token
                }
            }
            else{ //incorrectly formatted multi line comment
                //handle error, not sure how yet
            }
        }
        //single line comment
        else{
            //not a do while since the first character after # was already read and is possibly a EOF, \n or \r
            while(last_char != EOF && last_char != '\n' && last_char != '\r'){
                last_char = getchar();
            }
            //maybe this needs to be changed due to using while instead of do while?

            if (last_char != EOF){
                return get_token(); //again, skip whole line as a single token
            }
        }
    }
    //not consuming the EOF
    else if (last_char == EOF){
        return token_eof;
    }
    //otherwise, return the read character as ASCII
    else{
        int character_to_return = last_char;
        
        //consume the character
        last_char = getchar();

        return character_to_return;
    }
}

