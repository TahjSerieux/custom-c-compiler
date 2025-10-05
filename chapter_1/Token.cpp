#include<string>
#include "Token.hpp"
std::string token_to_string(TokenType type){
    switch (type)
    {
        case 0:
            return("KEYWORD");
            break;
        case 1:
            return("OPEN_PARENTHESIS");
            break;
        case 2:
            return("CLOSED_PARENTHESIS");
            break;
        case 3:
            return("OPEN_BRACKETS");
            break;
        case 4:
            return("CLOSED_BRACKETS");
            break;
        case 5:
            return("CONSTANTS");
            break;
        case 6:
            return("SEMICOLON");
            break;
        case 7:
            return("IDENTIFIER");
            break;
        default:
            return("UNKNOWN");
    }
}
Token::Token(std::string value, TokenType type){
    this->type =  type;
    this->value = value;
}
std::string Token::getValue(){
    return(this->value);
}
TokenType Token::getTokenType(){
    return(this->type);
}
