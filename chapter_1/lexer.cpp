#include <vector>
#include <string>
#include <unordered_set>
#include<iostream>
#include<stdexcept>

#include "AST.cpp"
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
        Token(std::string value, TokenType type){
            this->type =  type;
            this->value = value;
        }
        std::string getValue(){
            return(this->value);
        }
        TokenType getTokenType(){
            return(this->type);
        }
        

    private:
        std::string value;
        TokenType type;


};
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
bool isDigit(char& c){
    return(c >= '0'&&  c <='9');
}   
class Lexer{
    std::vector<Token> tokens;
    std::unordered_set<std::string> keywords={"int","return","void"};
    private:
        bool isWhiteSpace(char& c){
            return(c == ' '|| c=='\t'|| c=='\n'||c=='\r');
        }
        
        bool isLetter(char& c){
            return((c >= 'a' && c <='z')||(c>='A' && c<='Z') || c== '_');
        }
        std::string getSymbol(std::string::iterator& it, std::string::iterator end,TokenType type){
            std::string symbol;
            if(type == CONSTANTS){
                while(it != end &&isDigit(*it)){
                    symbol +=  *it;
                    it++;
                }
                if(it!= end && !isWhiteSpace(*it)){
                    if(isLetter(*it) || *it=='@'){

                        throw std::runtime_error("Invalid Symbol Detected: " + symbol +*it);
                    }
                }
            }else if(it != end &&type == IDENTIFIER){
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
        bool isKeyword(std::string symbol){
            if(keywords.find(symbol) == keywords.end()){
                return(false);
            }
            return(true);
        }
        
    public:
        void tokenize(std::string str){
            std::string::iterator it =  str.begin();
            while(it != str.end()){
                if(isWhiteSpace(*it)){
                    it++;
                }else if(*it == '('){
                    Token t("(",OPEN_PARENTHESIS);
                    tokens.push_back(t);
                    it++;
                }else if(*it == ')'){
                    Token t(")",CLOSED_PARENTHESIS);
                    tokens.push_back(t);
                    it++;
                }else if(*it == '{'){
                    Token t("{", OPEN_BRACKETS);
                    tokens.push_back(t);
                    it++;
                }else if(*it == '}'){
                    Token t("}", CLOSED_BRACKETS);
                    tokens.push_back(t);
                    it++;
                }else if(*it == ';'){
                    Token t(";",SEMICOLON);
                    tokens.push_back(t);
                    it++;
                }else if(isDigit(*it)){
                    //Expecting an valid number followed by a whitespace. (123L* is not valid L=[A-Z+a-z+_])
                    Token t{getSymbol(it, str.end(),CONSTANTS),CONSTANTS};
                    tokens.push_back(t);
                }else if(isLetter(*it) || *it =='_'){
                    //Expecting a Valid Identifier(words consisting of [A-Za-z]w*,w=[A-Z+a-z+0-9+_])
                    std::string symbol = getSymbol(it,str.end(),IDENTIFIER);
                    if(isKeyword(symbol)){
                        Token t(symbol,KEYWORD);
                        tokens.push_back(t);
                    }else{
                        Token t(symbol,IDENTIFIER);
                        tokens.push_back(t);
                    }
                }else{
                    throw std::runtime_error("Invalid Symbol Detected: "+std::string(1,*it));
                }
            }
        }
        void printTokens(){
            for(Token t: tokens){
                std::cout<<(token_to_string(t.getTokenType()))<<": "<<t.getValue()<<'\n';
            }
        }
        std::vector<Token> getTokens(){
            return(this->tokens);
        }

    };
class Parser{
    public:
        Parser(std::vector<Token> tokens){
            this->tokens = tokens;
            this->it =  this->tokens.begin();
        }
        
        ProgramNode* parseProgram(){
 
                /* code */
                return(new ProgramNode{parseFunction()});
         
        }
        

    private:
        std::vector<Token> tokens;
        std::vector<Token>::iterator it;
        void expect(TokenType type,std::string value =""){
            // std::cout<<"Expecting "<<token_to_string(type)<<" with value "<<value<<'\n';
            // std::cout<<"(it) is pointing to:\ntype "<<token_to_string(it->getTokenType())<<" with value "<<it->getValue()<<'\n';
            // std::cout<<"-------------------------------------------------------------------------------------------"<<'\n';
            if(it == this->tokens.end()){
                throw std::runtime_error("Reached the final token, no more tokens to parse");
            }
            if(it->getTokenType() != type || it->getValue() != value){
                throw std::runtime_error("Expected Terminal "+ it->getValue()+ "of type: "+ token_to_string(it->getTokenType())+" but got:  "+ value +" of type: "+ token_to_string(type));
            }
            it++;

        }
        std::string parseInt(){
            if( it->getTokenType() == CONSTANTS ){
                std::string value =it->getValue();
                it++;
                return(value);
            }
            throw std::runtime_error("Expected Terminal CONSTANT but got: "+ it->getValue() +" of type: "+token_to_string(it->getTokenType()));
        }
        
        ExpNode* parseExp(){
            std::string constant = parseInt();
            return (new ExpNode{constant});
        }
        StatementNode* parseStatement(){
            expect(KEYWORD,"return");
            ExpNode* exp =  parseExp();
            expect(SEMICOLON,";");
            // StatementNode* node = new StatementNode{exp};
            return(new StatementNode{exp});
        }
        std::string parseIdentifier(){
            if(it==tokens.end() || it->getTokenType() != IDENTIFIER){
                throw std::runtime_error("Expected token of type IDENTIFIER");
            }
            std::string str = it->getValue();
            it++;
            return(str);
        }
        FunctionDefinitionNode* parseFunction(){
            expect(KEYWORD,"int");
            std::string name = parseIdentifier();
            expect(OPEN_PARENTHESIS,"(");
            expect(KEYWORD,"void");
            expect(CLOSED_PARENTHESIS,")");
            expect(OPEN_BRACKETS,"{");
            StatementNode* function_body =  parseStatement();
            expect(CLOSED_BRACKETS,"}");
            return(new FunctionDefinitionNode{name,function_body});
        }
        



};