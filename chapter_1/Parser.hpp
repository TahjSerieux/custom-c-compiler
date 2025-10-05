#include<vector>
#include<string>
#include<stdexcept>
#include "AST.hpp"
#include"Token.hpp"
class Parser{
    public:
        Parser(std::vector<Token> tokens);
        
        std::vector<ProgramNode*> parseProgram();
        

    private:
        std::vector<Token> tokens;
        std::vector<Token>::iterator it;
        void expect(TokenType type,std::string value ="");
        std::string parseInt();
        
        ExpNode* parseExp();
        StatementNode* parseStatement();
        std::string parseIdentifier();
        FunctionDefinitionNode* parseFunction();
};