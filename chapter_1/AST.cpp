#include <string>
#include <vector>
#include <iostream>
#include "AST.hpp"
std::string unary_operator_to_string(UnaryOperator op){
    switch(op) {
        case UnaryOperator::Negation: return "-";
        case UnaryOperator::Complement: return "~";
    }
    return ""; // optional default, to silence compiler warnings
}
// ======================================================
//                     ExpressionNode
// ======================================================
//Abstract Base class for all expression(Statements that evaluate to a value);
ExpressionNode::ExpressionNode(ExpressionType t): type(t){}
ExpressionType ExpressionNode::getType() const {
    return(this->type);
}


// ======================================================
//                     ConstantNode::ExpressionNode
// ======================================================
ConstantNode::ConstantNode(std::string value):ExpressionNode(ExpressionType::CONSTANT), value(value){}
ConstantNode::~ConstantNode(){};
void ConstantNode::print(){
    std::cout<<"\t\t\tConstant ("<<value<<")";
}
const std::string ConstantNode::getValue(){
    return(this->value);
}
// ======================================================
//                     UnaryNode::ExperssionNode
// ======================================================
UnaryNode::UnaryNode(UnaryOperator unary_operator, ExpressionNode* exp):ExpressionNode{ExpressionType::UNARY},unary_operator(unary_operator),exp(exp){}

UnaryOperator UnaryNode::get_unary_operator(){
    return(this->unary_operator);
}

ExpressionNode* UnaryNode::getExpression(){
    return(this->exp);
}

void UnaryNode::print(){
    std::cout<<"\t\t"<<unary_operator_to_string(this->unary_operator) +this->exp->getValue();
}

const std::string UnaryNode::getValue(){
    return(unary_operator_to_string(this->unary_operator) + " " +this->exp->getValue());
    // return("");
}
UnaryNode::~UnaryNode(){}

// void UnaryNode::print(){
//     std::cout<<unary_operator_to_string(this->unary_operator);
//     this->exp->print();
// }


// ======================================================
//                     StatementNode
// ======================================================
//Abstract Base class for all statements(Self contained unit of execution/ fancy way of saying a syntatically correct line that does something)
StatementNode::StatementNode(StatementType t):type(t) {}
StatementType StatementNode::getType(){
    return(this->type);
}
StatementNode::~StatementNode() {}

// ======================================================
//                     ReturnNode::StatementNode
// ======================================================
//ReturnNode
ReturnNode::ReturnNode(ExpressionNode* exp):StatementNode(StatementType::RETURN), exp(exp){}
void ReturnNode::print(){
    std::cout<<"Return(\n";
    this->exp->print();
    std::cout<<"\n\t\t)";
}

ExpressionNode* ReturnNode::getExpression(void)const{
    return(this->exp);
}

// ======================================================
//                     FunctionNode
// ======================================================
//FunctionNode
FunctionNode::FunctionNode(std::string identifier, StatementNode* statement){
    this->identifier =  identifier;
    this->statement =  statement;
}
void FunctionNode::print(){
    // std::cout<<this->statement<<'\n';
    std::cout<<"\tFunction(\n";
    std::cout<<"\t\tname=\""<<this->identifier<<"\",\n\t\tbody=";
    this->statement->print();
    std::cout<<"\n\t)\n";
}
std::string FunctionNode::getIdentifer(){
    return(this->identifier);
}
StatementNode* FunctionNode::getStatement(){
    return(this->statement);
}

// ======================================================
//                     ProgramNode
// ======================================================
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

       

// ======================================================
//                     AST
// ======================================================
AST::AST(ProgramNode* root):root(root){}

void AST::PrettyPrint() const{
    root->print();
}
        
const ProgramNode* AST::getRoot() const{
    return(this->root);
}