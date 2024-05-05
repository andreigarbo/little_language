#include <fstream>

class Lexer {
private:
    int last_char;
    int get_token();
    std::ifstream file;
public:
    Lexer();
    void setFilename(const std::string& filename);
    int get_next_token_from_input();
    double numeric_value_float;
    int numeric_value_int;
    std::string identifier_string;
};

