#include <string>
#include <vector>
#include <iostream>
#include "AST.hpp"

//Abstract Base class for all expression(Statements that evaluate to a value);

ExpressionNode::ExpressionNode(ExpressionType t): type(t){}
ExpressionType ExpressionNode::getType() const {
    return(this->type);
}

ConstantNode::ConstantNode(std::string value):ExpressionNode(CONSTANT), value(value){}
ConstantNode::~ConstantNode(){};
void ConstantNode::print(){
    std::cout<<"\t\t\tConstant ("<<value<<")";
}
const std::string ConstantNode::getValue(){
    return(this->value);
}

//Abstract Base class for all statements(Self contained unit of execution/ fancy way of saying a syntatically correct line that does something)
StatementNode::StatementNode(StatementType t):type(t) {}
StatementType StatementNode::getType(){
    return(this->type);
}
StatementNode::~StatementNode() {}


//ReturnNode
ReturnNode::ReturnNode(ExpressionNode* exp):StatementNode(RETURN), exp(exp){}
void ReturnNode::print(){
    std::cout<<"Return(\n";
    this->exp->print();
    std::cout<<"\n\t\t)";
}

ExpressionNode* ReturnNode::getExpression(void)const{
    return(this->exp);
}
//FunctionNode
FunctionNode::FunctionNode(std::string identifier, StatementNode* statement){
    this->identifier =  identifier;
    this->statement =  statement;
}
void FunctionNode::print(){
    // std::cout<<this->statement<<'\n';
    std::cout<<"\tFunction(\n";
    std::cout<<"\t\tname=\""<<this->identifier<<"\",\n\t\t";
    this->statement->print();
    std::cout<<"\n\t)\n";
}
std::string FunctionNode::getIdentifer(){
    return(this->identifier);
}
StatementNode* FunctionNode::getStatement(){
    return(this->statement);
}


ProgramNode::ProgramNode(std::vector<FunctionNode*> funcs){
    this->functions =  funcs;
}
void ProgramNode::print() const{
    std::cout<<"Program(\n";
        for(FunctionNode* f: functions){
            f->print();
        }
    std::cout<<"\n)\n";
}
std::vector<FunctionNode*> ProgramNode::getFunction() const{
    return(this->functions);
}

       


AST::AST(ProgramNode* root):root(root){}

void AST::PrettyPrint() const{
    root->print();
}
        
const ProgramNode* AST::getRoot() const{
    return(this->root);
}