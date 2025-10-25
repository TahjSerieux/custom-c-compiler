#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <unordered_set>
enum TokenType{
    KEYWORD,
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,
    OPEN_BRACKETS,
    CLOSED_BRACKETS,
    CONSTANTS,
    SEMICOLON,
    IDENTIFIER,
    DECREMENT,
    TILDE,
    HYPHEN
};
class Token{
    public:
    Token(){

        }
        Token(std::string value, TokenType type);
        std::string getValue();
        enum TokenType getTokenType();

    private:
        std::string value;
        TokenType type;


};

std::string token_to_string(TokenType type);
bool isUnaryOperator(Token t);

#endif // TOKEN_HPP