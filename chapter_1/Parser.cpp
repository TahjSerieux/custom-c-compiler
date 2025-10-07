#include "Parser.hpp"
Parser::Parser(std::vector<Token> tokens){
            this->tokens = tokens;
            this->it =  this->tokens.begin();
        }
        
ProgramNode* Parser::parseProgram(){

        /* code */
        // ProgramNode* program =new ProgramNode{parseFunction()};
        // if(it<this->tokens.end()){
        //     throw std::runtime_error("Error Parsing");
        // }
        std::vector<FunctionNode*> functions;
        while(it != this->tokens.end()){
            functions.push_back(parseFunction());
            // std::cout<<"THE FUNCTIONS IDENTIFER IS: "<<functions[functions.size()-1]->getIdentifer()<<'\n';
        }

        ProgramNode* AST_Root = new ProgramNode{functions};
        return(AST_Root);
    
}

void Parser::expect(TokenType type,std::string value){
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

std::string Parser::parseIdentifier(){
    if(it==tokens.end() || it->getTokenType() != IDENTIFIER){
        throw std::runtime_error("Expected token of type IDENTIFIER");
    }
    std::string str = it->getValue();
    it++;
    return(str);
}
std::string Parser::parseInt(){
    if(it == this->tokens.end()){
        throw std::runtime_error("Error processing tokens: Unexepectedly reach end of tokens");
    }
    if(it!= this->tokens.end() && it->getTokenType() == CONSTANTS ){
        std::string value =it->getValue();
        it++;
        return(value);
    }

    throw std::runtime_error("Expected Terminal CONSTANT but got: "+ it->getValue() +" of type: "+token_to_string(it->getTokenType()));
}
ExpressionNode* Parser::parseExpression(){
    //If the current expression is a constant integer value i.e (54)
    std::string constant = parseInt();
    //Need to implement binary operators in  the future
    return (new ConstantNode{constant});
}
StatementNode* Parser::parseStatement(){
    expect(KEYWORD,"return");
    
    ExpressionNode* exp =  parseExpression();
    expect(SEMICOLON,";");
    // StatementNode* node = new StatementNode{exp};
    return(new ReturnNode{exp});
}
FunctionNode* Parser::parseFunction(){
    expect(KEYWORD,"int");
    std::string name = parseIdentifier();

    expect(OPEN_PARENTHESIS,"(");
    expect(KEYWORD,"void");
    expect(CLOSED_PARENTHESIS,")");
    expect(OPEN_BRACKETS,"{");
    StatementNode* function_body =  parseStatement();

    expect(CLOSED_BRACKETS,"}");
    return(new FunctionNode{name,function_body});
}
