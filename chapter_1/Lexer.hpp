#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <string>
#include <unordered_set>
#include<iostream>
#include<stdexcept>

#include "Token.hpp"

class Lexer{
    std::vector<Token> tokens;
    std::unordered_set<std::string> keywords={"int","return","void"};
    private:
        //Checks if the current character in the string is a whitespace
        bool isWhiteSpace(char& c);
        
        bool isLetter(char& c);
        bool isDigit(char& c);
        std::string getSymbol(std::string::iterator& it, std::string::iterator end,TokenType type);
        bool isKeyword(std::string symbol);
        
    public:
        void tokenize(std::string str);
        void printTokens();
        std::vector<Token> getTokens();

    };
#endif