#include "Lexer.h"

//function for getting tokens from the input
int Lexer::get_token(){
    //skips any and all whitespace characters present before the token
    while(isspace(last_char)){
        last_char = file.get();
    }

    //characters in range [a-zA-Z]*
    if (isalpha(last_char)){
        identifier_string = last_char;

        //while characters read are in the above mentioned range
        while (isalnum(last_char = file.get()) || last_char == '_') {
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
        else if(identifier_string == "from"){
            return token_from;
        }
        else if(identifier_string == "to"){
            return token_to;
        }
        else if(identifier_string == "global"){
            return token_global;
        }
        else if(identifier_string == "import"){
            return token_import;
        }
        else if(identifier_string == "return"){
            return token_return;
        }
        else if(identifier_string == "void"){
            return token_void;
        }
        else if(identifier_string == "iterate"){
            return token_iterate;
        }

        //if not a specially defined keyword, return token_identifier to signal a custom word (such as a variable name)
        return token_identifier; 
    }

    //for int numbers or float numbers, format 1.2345.....
    else if (isdigit(last_char) || last_char == '.' || last_char == '-'){ 
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
                last_char = file.get();
            }
            //this skips any further decimal points, making 1.23.45 equal to 1.2345
            else if(last_char =='.' && has_read_decimal_point == true){
                last_char = file.get();
            }
            //this case is for numerical characters
            else{
                read_number += last_char;
                last_char = file.get();
            }
        } while (isdigit(last_char) || last_char == '.');

        //strtod takes 2 arguments, first the value to be converted to a floating point number
        //and then a pointer where it can store any characters that are not numeric (nullptr should discard any non numeric characters)
        //string.c_str() converts a string to a null terminated character array
        if (!has_read_decimal_point){
            numeric_value_int = strtol(read_number.c_str(), nullptr, 10);
            return token_int_number;
        } else {
            numeric_value_float = strtod(read_number.c_str(), nullptr);
            return token_float_number;
        }
    }
    //single line comments start with #
    //multi line comments start and end with ##
    else if (last_char == '#'){
        last_char = file.get();
        //multi line comment
        if(last_char == '#'){
            do{
                last_char = file.get();
            } while(last_char != '#');
            last_char = file.get();
            if(last_char == '#'){ // correctly formatted multi line comment
                last_char = file.get();
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
                last_char = file.get();
            }
            //maybe this needs to be changed due to using while instead of do while?

            if (last_char != EOF){
                return get_token(); //again, skip whole line as a single token
            }
        }
    }
    else if (last_char == '<' || last_char == '>' || last_char == '=' || last_char == '!') {
        int next_char = file.get();
        while(isspace(next_char)){
            next_char = file.get();
        }
        if (next_char == '=') {
            if (last_char == '<'){
                last_char = file.get();
                return token_le;
            }
            else if (last_char == '>'){
                last_char = file.get();
                return token_ge;
            }
            else if (last_char == '='){
                last_char = file.get();
                return token_eq;
            }
            else if (last_char == '!'){
                last_char = file.get(); 
                return token_ne;
            }
        }    
        else {
            //if not a 2 char compound operator, meaning either '!' or '='
            int character_to_return = last_char;
            last_char = next_char;
            return character_to_return;
        }   
    }
    else if (last_char == '\''){
        last_char = file.get();
        if (last_char == '\''){
            identifier_string = "";
            return token_string_value;
        }
        identifier_string = last_char;
        last_char = file.get();
        while (last_char != '\''){
            identifier_string += last_char;
            last_char = file.get();
        }
        last_char = file.get();
        return token_string_value;
    }
    else if (last_char == '\"'){
        last_char = file.get();
        identifier_string = last_char;
        last_char = file.get();
        while (last_char != '\"'){
            identifier_string += last_char;
            last_char = file.get();
        }
        last_char = file.get();
        return token_string_value;
    }
    //not consuming the EOF
    else if (last_char == EOF){
        return token_eof;
    }
    //otherwise, return the read character as ASCII
    else{
        int character_to_return = last_char;
        
        //consume the character
        last_char = file.get();

        return character_to_return;
    }
    return -1;
}

int Lexer::get_next_token_from_input() {
    int ct;
    ct = get_token();
    return ct;
   // return get_token();
}

void Lexer::setFilename(const std::string& filename){
    file.open(filename);
}