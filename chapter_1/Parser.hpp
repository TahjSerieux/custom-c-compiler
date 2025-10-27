#ifndef PARSER_H
#define PARSER_H
#include<vector>
#include<string>
#include<stdexcept>
#include "AST.hpp"
#include"Token.hpp"
class Parser{
    public:
        Parser(std::vector<Token> tokens);
        
        ProgramNode* parseProgram();
        

    private:
        std::vector<Token> tokens;
        std::vector<Token>::iterator it;
        // ProgramNode* root;
        /*
         if the current Token matches the expected token based on the syntax of the language. Auto advances the iterator 
         to the next Token performs error checking.
        */
        void expect(TokenType type,std::string value ="");
        std::string parseInt();
        UnaryNode* parseUnaryExpression();
        ExpressionNode* parseExpression();
        StatementNode* parseStatement();
        std::string parseIdentifier();
        FunctionNode* parseFunction();
        std::vector<Token>::iterator parserPeek(int pos);
        int precedneceChecker(Token t);
};
#endif
