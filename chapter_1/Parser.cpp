#include "Parser.hpp"

void Parser::consumeToken(){
    this->it++;
}
std::unordered_set<TokenType> unaryOperators = {TokenType::HYPHEN,TokenType::TILDE};
bool Parser::isTokenAUnaryOperator(){
    if(unaryOperators.find(it->getTokenType()) == unaryOperators.end()){
        return(false);
    }
    return(true);
}
bool Parser::isBinaryOperator(){
    TokenType type = it->getTokenType();
    switch (type)
    {
        case TokenType::ADD:
            // return(BinaryOperator::Add);
        case TokenType::MUL:
            // return(BinaryOperator::Multiply);
        case TokenType::DIV:
            // return(BinaryOperator::Divide);
        case TokenType::MOD:
            // return(BinaryOperator::Remainder);
        case TokenType::HYPHEN:
            // return(BinaryOperator::Subtract);
            return(true);
        default:
            return(false);
    }
}
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
    // it++;
    consumeToken();

}

std::string Parser::parseIdentifier(){
    if(it==tokens.end() || it->getTokenType() != TokenType::IDENTIFIER){
        throw std::runtime_error("Expected token of type IDENTIFIER");
    }
    std::string str = it->getValue();
    // it++;
    consumeToken();
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
        // unary = UnaryOperator::Error;
        throw std::runtime_error("Unkown operator");
    }
    // it++;
    consumeToken();
    return(new UnaryNode{unary,parseExpression()});
}

std::string Parser::parseInt(){
    if(it == this->tokens.end()){
        throw std::runtime_error("Runtime error fond in std::string Parser::parseInt(): Error processing tokens: Unexepectedly reach end of tokens");
    }
    if(it!= this->tokens.end() && it->getTokenType() == TokenType::CONSTANTS ){
        std::string value =it->getValue();
        // it++;
        consumeToken();
        return(value);
    }
    
    throw std::runtime_error("Runtime error fond in std::string Parser::parseInt(): Expected Terminal CONSTANT but got: "+ it->getValue() +" of type: "+token_to_string(it->getTokenType()));
}
ExpressionNode* Parser::parseFactor(){
    TokenType type =  it->getTokenType();
    ExpressionNode* node = nullptr;
     //<int>
    if(type == TokenType::CONSTANTS){
        std::string value = parseInt();
        return(new ConstantNode{value});
    }//<unop><exp>
    else if(isTokenAUnaryOperator() ){
        std::cout<<"UNARY OPERATOR\n";
        UnaryOperator op = parseUnaryOperator();
        // std::cout<<unary_operator_to_string(op)<<'\n';
        // consumeToken();
        std::cout<<it->getValue()<<'\n';
        ExpressionNode* expr = parseFactor();
        std::cout<<"Got expression \n";
        return(new UnaryNode{op,expr});

    }//"("<exp>")"
    else if(type == TokenType::OPEN_PARENTHESIS){
        consumeToken();
        ExpressionNode* expr = parseExpression();
        expect(TokenType::CLOSED_PARENTHESIS,")");
        return(expr);
    }else{
        throw std::runtime_error("[ExpressionNode* Parser::parseFactor()]: Improper Factor");
    }
}

ExpressionNode* Parser::parseExpression(int minPrecedence){
    std::cout<<"BEFORE PASRSE FACTOR\n";
    ExpressionNode* lhs = parseFactor();

    while(isBinaryOperator() && getPrecedence()>= minPrecedence){
        int operatorPrecedence = getPrecedence();

        BinaryOperator op =  parseBinaryOperator();

        ExpressionNode* rhs =  parseExpression(operatorPrecedence+1);
        lhs = new BinaryNode{lhs,op,rhs};
    }
    std::cout<<"AFTER PASRSE FACTOR\n";
    return(lhs);
}

ExpressionNode* Parser::parseExpression(){
    //If the current expression is a constant integer value i.e (54)
    if(parserPeek(0)->getTokenType() == TokenType::CONSTANTS ){
        std::string constant = parseInt();
        return (new ConstantNode{constant});
    }else if(isTokenAUnaryOperator()){
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
    
    // ExpressionNode* exp =  parseExpression();
    ExpressionNode* exp =  parseExpression(0);
    expect(TokenType::SEMICOLON,";");
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


int Parser::getPrecedence(){
    TokenType type = it->getTokenType();
    if(firstPrecedence.find(type)!= firstPrecedence.end()){
        return(1);
    }else if(secondPrecedence.find(type) != secondPrecedence.end() ){
        return(2);
    }else if(thirdPrecedence.find(type) != thirdPrecedence.end()){
        return(3);
    }else{
        throw std::runtime_error("[int Parser::getPrecedence()]: UNDEFINED PRECEDENCE");
    }
}

// std::string Parser::precedenceClimber(){

// }
BinaryOperator Parser::parseBinaryOperator(){
    TokenType type = it->getTokenType();
    consumeToken();
    switch (type)
    {
    case TokenType::ADD:
        return(BinaryOperator::Add);
    case TokenType::MUL:
        return(BinaryOperator::Multiply);
    case TokenType::DIV:
        return(BinaryOperator::Divide);
    case TokenType::MOD:
        return(BinaryOperator::Remainder);
    case TokenType::HYPHEN:
        return(BinaryOperator::Subtract);
    default:
        throw std::runtime_error("Invalid Binary Operator");
    }
}
// BinaryNode* Parser::parseBinary(){
//     std::string firstOperand = parseInt();

// }
UnaryOperator Parser::parseUnaryOperator(){
    TokenType type =  it->getTokenType();
    consumeToken();
    switch (type)
    {
        case TokenType::HYPHEN:
            return(UnaryOperator::Negation);
            break;
        case TokenType::TILDE:
            return(UnaryOperator::Complement);
            break;
        default:
            throw std::runtime_error("Invalid Unary Operator");
    }
}
