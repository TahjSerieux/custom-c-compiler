#include "Parser.hpp"
Parser::Parser(std::vector<Token> tokens){
            this->tokens = tokens;
            this->it =  this->tokens.begin();
        }
        
ProgramNode* Parser::parseProgram(){


        std::vector<FunctionNode*> functions;
        while(it != this->tokens.end()){
            functions.push_back(parseFunction());
        }

        ProgramNode* AST_Root = new ProgramNode{functions};
        return(AST_Root);
    
}

void Parser::expect(TokenType type,std::string value){
    if(it == this->tokens.end()){
        throw std::runtime_error("Reached the final token, no more tokens to parse");
    }
    if(it->getTokenType() != type || it->getValue() != value){
        throw std::runtime_error("Expected Terminal "+ it->getValue()+ " of type: "+ token_to_string(it->getTokenType())+" but got:  "+ value +" of type: "+ token_to_string(type));
    }
    it++;

}

std::string Parser::parseIdentifier(){
    if(it==tokens.end() || it->getTokenType() != TokenType::IDENTIFIER){
        throw std::runtime_error("Expected token of type IDENTIFIER");
    }
    std::string str = it->getValue();
    it++;
    return(str);
}
UnaryNode* Parser::parseUnaryExpression(){
    TokenType type = this->it->getTokenType();
    UnaryOperator unary;
    if(type  == TokenType::HYPHEN){
        unary = UnaryOperator::Negation;
    }else if(type == TokenType::TILDE){
        unary = UnaryOperator::Complement;
    }else{
        unary = UnaryOperator::Error;
    }
    it++;
    return(new UnaryNode{unary,parseExpression()});
    // return(nullptr);
}
std::string Parser::parseInt(){
    if(it == this->tokens.end()){
        throw std::runtime_error("Error processing tokens: Unexepectedly reach end of tokens");
    }
    if(it!= this->tokens.end() && it->getTokenType() == TokenType::CONSTANTS ){
        std::string value =it->getValue();
        it++;
        return(value);
    }

    throw std::runtime_error("Expected Terminal CONSTANT but got: "+ it->getValue() +" of type: "+token_to_string(it->getTokenType()));
}
ExpressionNode* Parser::parseExpression(){
    //If the current expression is a constant integer value i.e (54)
    if(parserPeek(0)->getTokenType() == TokenType::CONSTANTS ){
        std::string constant = parseInt();
        return (new ConstantNode{constant});
    }else if(isUnaryOperator(*parserPeek(0))){
        UnaryNode* node =  parseUnaryExpression();
        // node->print();std::cout<<'\n';
        return(node);
    }else if(parserPeek(0)->getTokenType() == TokenType::OPEN_PARENTHESIS){
        expect(TokenType::OPEN_PARENTHESIS,"("); 
        ExpressionNode* node = parseExpression();
        expect(TokenType::CLOSED_PARENTHESIS,")"); 
        return(node);
    }else{
        throw std::runtime_error("Malformed Expression");\
    }
    //Need to implement binary operators in  the future
    // return (new ConstantNode{"constant"});
}
StatementNode* Parser::parseStatement(){
    expect(TokenType::KEYWORD,"return");
    
    ExpressionNode* exp =  parseExpression();
    expect(TokenType::SEMICOLON,";");
    // StatementNode* node = new StatementNode{exp};
    return(new ReturnNode{exp});
}
FunctionNode* Parser::parseFunction(){
    expect(TokenType::KEYWORD,"int");
    std::string name = parseIdentifier();
    expect(TokenType::OPEN_PARENTHESIS,"(");
    expect(TokenType::KEYWORD,"void");
    expect(TokenType::CLOSED_PARENTHESIS,")");
    expect(TokenType::OPEN_BRACKETS,"{");
    StatementNode* function_body =  parseStatement();

    expect(TokenType::CLOSED_BRACKETS,"}");
    return(new FunctionNode{name,function_body});
}

std::vector<Token>::iterator Parser::parserPeek(int pos) {
    auto it2 = it;
    std::advance(it2, pos);
    if (it2 >= tokens.end())
        throw std::runtime_error("Attempted to peek past end of tokens");
    return it2;
}


int Parser::precedneceChecker(Token t){
    TokenType type = t.getTokenType();
    if(firstPrecedence.find(type)!= firstPrecedence.end()){
        return(1);
    }else if(secondPrecedence.find(type) != secondPrecedence.end() ){
        return(2);
    }else if(thirdPrecedence.find(type) != thirdPrecedence.end()){
        return(3);
    }else{
        throw std::runtime_error("UNDEFINED PRECEDENCE");
    }
}