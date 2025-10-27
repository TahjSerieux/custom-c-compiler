#include <vector>
#include <string>
#include <unordered_set>
#include<iostream>
#include<stdexcept>

// #include "AST.hpp"

#include "Token.hpp"
#include "Lexer.hpp"

std::string::iterator Lexer::lexerPeek(std::string::iterator& it,int pos){
    std::string::iterator it2 =  it +pos;
    // std::cout<<"it2: "<<*it2<<'\n';
    return(it2);
}
bool Lexer::isWhiteSpace(char& c){
    return(c == ' '|| c=='\t'|| c=='\n'||c=='\r');
}

bool Lexer::isLetter(char& c){
    return((c >= 'a' && c <='z')||(c>='A' && c<='Z') || c== '_');
}
bool Lexer::isDigit(char& c){
    return(c >= '0'&&  c <='9');
}   
std::string Lexer::createSymbol(std::string::iterator& it, std::string::iterator end,TokenType type){
    std::string symbol;
    if(type == TokenType::CONSTANTS){
        while(it != end &&isDigit(*it)){
            symbol +=  *it;
            it++;
        }
        if(it!= end && !isWhiteSpace(*it)){
            if(isLetter(*it) || *it=='@'){

                throw std::runtime_error("Invalid Symbol Detected: " + symbol +*it);
            }
        }
    }else if(it != end &&type == TokenType::IDENTIFIER){
        while( it != end && (isLetter(*it) ||isDigit(*it))){
            symbol += *it;
            it++;
        }
        if(symbol == ""){
            throw std::runtime_error("Invalid Symbol Detected: " + symbol +*it);

        }
    }
    return(symbol);
}
bool Lexer::isKeyword(std::string symbol){
    if(keywords.find(symbol) == keywords.end()){
        return(false);
    }
    return(true);
}

void Lexer::tokenize(std::string str){
    std::string::iterator it =  str.begin();
    while(it != str.end()){
        if(isWhiteSpace(*it)){
            it++;
        }else if(*it == '('){
            Token t("(",TokenType::OPEN_PARENTHESIS);
            tokens.push_back(t);
            it++;
        }else if(*it == ')'){
            Token t(")",TokenType::CLOSED_PARENTHESIS);
            tokens.push_back(t);
            it++;
        }else if(*it == '{'){
            Token t("{", TokenType::OPEN_BRACKETS);
            tokens.push_back(t);
            it++;
        }else if(*it == '}'){
            Token t("}", TokenType::CLOSED_BRACKETS);
            tokens.push_back(t);
            it++;
        }else if(*it == ';'){
            Token t(";",TokenType::SEMICOLON);
            tokens.push_back(t);
            it++;
        }else if(*it == '-' && *lexerPeek(it,1)!='-'){
            Token t("-",TokenType::HYPHEN);
            tokens.push_back(t);
            it++;
        }else if(*it == '-' && *lexerPeek(it,1)=='-'){
            Token t("--",TokenType::DECREMENT);
            tokens.push_back(t);
            it+=2;
        }else if(*it == '~'){
            Token t("~",TokenType::TILDE);
            tokens.push_back(t);
            it++;
        }else if(*it == '+'){
            Token t("+",TokenType::ADD);
            tokens.push_back(t);
            it++;
        }else if(*it == '*'){
            Token t("*",TokenType::MUL);
            tokens.push_back(t);
            it++;
        }else if(*it == '/'){
            Token t("/",TokenType::DIV);
            tokens.push_back(t);
            it++;
        }else if(*it == '%'){
            Token t("%",TokenType::MOD);
            tokens.push_back(t);
            it++;
        }
        else if(isDigit(*it)){
            //Expecting an valid number followed by a whitespace. (123L* is not valid L=[A-Z+a-z+_])
            Token t(createSymbol(it, str.end(),TokenType::CONSTANTS),TokenType::CONSTANTS);
            tokens.push_back(t);
        }else if(isLetter(*it) || *it =='_'){
            // std::cout<<"Symbol: "<<*it<<'\n';
            //Expecting a Valid Identifier(words consisting of [A-Za-z]w*,w=[A-Z+a-z+0-9+_])
            std::string symbol = createSymbol(it,str.end(),TokenType::IDENTIFIER);
            if(isKeyword(symbol)){
                Token t(symbol,TokenType::KEYWORD);
                tokens.push_back(t);
            }else{
                Token t(symbol,TokenType::IDENTIFIER);
                tokens.push_back(t);
            }
        }else{
            throw std::runtime_error("Invalid Symbol Detected: "+std::string(1,*it));
        }
    }
}
void Lexer::printTokens(){
    for(Token t: tokens){
        std::cout<<(token_to_string(t.getTokenType()))<<": "<<t.getValue()<<'\n';
        
    }
}
std::vector<Token> Lexer::getTokens(){
    return(this->tokens);
}


