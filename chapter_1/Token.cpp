#include "Token.hpp"
std::string token_to_string(TokenType type){
    switch (type)
    {
        case TokenType::KEYWORD:
            return("KEYWORD");
            break;
        case TokenType::OPEN_PARENTHESIS:
            return("OPEN_PARENTHESIS");
            break;
        case TokenType::CLOSED_PARENTHESIS:
            return("CLOSED_PARENTHESIS");
            break;
        case TokenType::OPEN_BRACKETS:
            return("OPEN_BRACKETS");
            break;
        case TokenType::CLOSED_BRACKETS:
            return("CLOSED_BRACKETS");
            break;
        case TokenType::CONSTANTS:
            return("CONSTANTS");
            break;
        case TokenType::SEMICOLON:
            return("SEMICOLON");
            break;
        case TokenType::IDENTIFIER:
            return("IDENTIFIER");
            break;
        case TokenType::DECREMENT:
            return("DECREMENT");
            break;
        case TokenType::TILDE:
            return("TILDE");
            break;
        case TokenType::HYPHEN:
            return("HYPHEN");
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
enum TokenType Token::getTokenType(){
    return(this->type);
}

std::unordered_set<TokenType> unaryOperators = {TokenType::HYPHEN,TokenType::TILDE};
bool isUnaryOperator(Token t){
    if(unaryOperators.find(t.getTokenType()) == unaryOperators.end()){
        return(false);
    }
    return(true);
}