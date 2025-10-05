#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
enum TokenType{
    KEYWORD,
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,
    OPEN_BRACKETS,
    CLOSED_BRACKETS,
    CONSTANTS,
    SEMICOLON,
    IDENTIFIER
};

class Token{
    public:
        Token(){

        }
        Token(std::string value, TokenType type);
        std::string getValue();
        TokenType getTokenType();

    private:
        std::string value;
        TokenType type;


};

std::string token_to_string(TokenType type);

#endif // TOKEN_HPP