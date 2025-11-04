#include <string>
#include <vector>
#include <iostream>
#include "AST.hpp"
// Helper function to handle indentation easily
static void printIndent(int indent) {
    for (int i = 0; i < indent; ++i) std::cout << "    "; // 4 spaces per indent
}
std::string unary_operator_to_string(UnaryOperator op){
    switch(op) {
        case UnaryOperator::Negation: return "-";
        case UnaryOperator::Complement: return "~";
        case UnaryOperator::Error: return "Error";
        case UnaryOperator::Unkown: return "Unkown";
    }
    throw std::runtime_error("std::string unary_operator_to_string(UnaryOperator op): No matching operator");
    // return ""; // optional default, to silence compiler warnings
}

std::string binary_operator_to_string(BinaryOperator op){
    switch (op)
    {
        case BinaryOperator::Add: return "+";
        case BinaryOperator::Subtract: return "-";
        case BinaryOperator::Multiply: return "*";
        case BinaryOperator::Divide: return "/";
        case BinaryOperator::Remainder: return "%";
    }
    // return("No Matching Operator")
    throw std::runtime_error("std::string binary_Operator_to_string(BinaryOperator op): No matching operator");
    
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
void ConstantNode::print(int indent) {
    printIndent(indent);
    std::cout << "Constant(\"" << value << "\")";
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

void UnaryNode::print(int indent) {
    printIndent(indent);
    std::cout << "UnaryNode(\n";
    printIndent(indent + 1);
    std::cout << "op=\"" << unary_operator_to_string(unary_operator) << "\",\n";
    printIndent(indent + 1);
    std::cout << "expr=\n";
    exp->print(indent + 2);
    std::cout << "\n";
    printIndent(indent);
    std::cout << ")";
}


// ======================================================
//                     BinaryNode:ExpressionNode
// ======================================================
BinaryNode::BinaryNode(ExpressionNode* firstExpression,BinaryOperator binary_operator,ExpressionNode* secondExpression):ExpressionNode{ExpressionType::BINARY},
binary_operator(binary_operator),firstExpression(firstExpression),secondExpression(secondExpression){}

BinaryNode::~BinaryNode()
{
}

ExpressionNode* BinaryNode::getFirstExpression(){
    return(this->firstExpression);
}
ExpressionNode* BinaryNode::getSecondExpression(){
    return(this->secondExpression);
}
BinaryOperator BinaryNode::getBinaryOperator(){
    return(this->binary_operator);
}
void BinaryNode::print(){
    // std::cout<<"BINARY NODE";
    std::cout<<"\t\t\t(\n";
    firstExpression->print();
    std::cout<<"\n\t\t\t"<<binary_operator_to_string(this->binary_operator)<<'\n';
    secondExpression->print();
    std::cout<<"\n\t\t\t)\n";

}

const std::string BinaryNode::getValue(){
    return("");
}

void BinaryNode::print(int indent) {
    printIndent(indent);
    std::cout << "BinaryNode(\n";
    printIndent(indent + 1);
    std::cout << "op=\"" << binary_operator_to_string(binary_operator)
              << "\", precedence=" << static_cast<int>(binary_operator)
              << ",\n";

    printIndent(indent + 1);
    std::cout << "lhs=\n";
    firstExpression->print(indent + 2);
    std::cout << ",\n";

    printIndent(indent + 1);
    std::cout << "rhs=\n";
    secondExpression->print(indent + 2);
    std::cout << "\n";

    printIndent(indent);
    std::cout << ")";
}


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
void ReturnNode::print(int indent) {
    printIndent(indent);
    std::cout << "Return(\n";
    exp->print(indent + 1);
    std::cout << "\n";
    printIndent(indent);
    std::cout << ")";
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


void FunctionNode::print(int indent) {
    printIndent(indent);
    std::cout << "Function(\n";
    printIndent(indent + 1);
    std::cout << "name=\"" << identifier << "\",\n";
    printIndent(indent + 1);
    std::cout << "body=\n";
    statement->print(indent + 2);
    std::cout << "\n";
    printIndent(indent);
    std::cout << ")";
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
std::vector<FunctionNode*> ProgramNode::getFunctions() const{
    return(this->functions);
}

       
void ProgramNode::print(int indent) const {
    printIndent(indent);
    std::cout << "Program(\n";
    for (auto* f : functions) {
        f->print(indent + 1);
        std::cout << "\n";
    }
    printIndent(indent);
    std::cout << ")";
}

// ======================================================
//                     AST
// ======================================================
AST::AST(ProgramNode* root):root(root){}
AST::AST():root(nullptr){}
void AST::PrettyPrint() const{
    if(root){
        root->print(0);
    }else{
        std::cout<<"<empty AST>\n";
    }
}
        
const ProgramNode* AST::getRoot() const{
    return(this->root);
}