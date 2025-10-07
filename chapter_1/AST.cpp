#include <string>
#include <vector>
#include <iostream>
// #include "AST.hpp"
#include "Lexer.hpp"
enum ExpressionType{CONSTANT};
enum StatementType{RETURN};
//Abstract Base class for all expression(Statements that evaluate to a value);
class ExpressionNode{
    private:

    public:
        virtual void print() = 0;
    protected:
        ExpressionType type;
        ExpressionNode(ExpressionType t): type(t){}

};
class ConstantNode:public ExpressionNode{
    private:
    std::string value;
    public:
    ~ConstantNode(){};
    ConstantNode(std::string value):ExpressionNode(CONSTANT), value(value){}
    void print() override{
        std::cout<<"\t\t\tConstant ("<<value<<")";
    }
};

//Abstract Base class for all statements(Self contained unit of execution/ fancy way of saying a syntatically correct line that does something)
class StatementNode{
    private:
    public:
        virtual ~StatementNode() = 0;
        virtual void print() = 0;
    /* data members can also be directly accessed by member functions and friends of classes that 
        are derived from the class that originally declared these members.*/
    protected:
        const StatementType type;
        StatementNode(StatementType t):type(t) {}
};
inline StatementNode::~StatementNode() {}
class ReturnNode: public StatementNode{
    private:
        ExpressionNode* exp;
    public:
        // ~ReturnNode(){};
        ReturnNode(ExpressionNode* exp):StatementNode(RETURN), exp(exp){}
        void print(){
            std::cout<<"Return(\n";
            this->exp->print();
            std::cout<<"\n\t\t)";
        }
};  
class FunctionNode{
    private:
        std::string identifier;
        StatementNode* statement;
    public:
        FunctionNode(std::string identifier, StatementNode* statement){
            this->identifier =  identifier;
            this->statement =  statement;
        }
        void print(){
            // std::cout<<this->statement<<'\n';
            std::cout<<"\tFunction(\n";
            std::cout<<"\t\tname=\""<<this->identifier<<"\",\n\t\t";
            this->statement->print();
            std::cout<<"\n\t)\n";
        }
        std::string getIdentifer(){
            return(this->identifier);
        }
        StatementNode* getStatement(){
            return(this->statement);
        }
};
    
class ProgramNode{
    private:
        // FunctionNode* function;
        std::vector<FunctionNode*> functions;
    public:
        // ProgramNode(FunctionNode* func){
        //     this->function = func;
        // }
        ProgramNode(std::vector<FunctionNode*> funcs){
            this->functions =  funcs;
        }
        void print(){
            std::cout<<"Program(\n";
                for(FunctionNode* f: functions){
                    f->print();
                }
            std::cout<<"\n)";
        }
        std::vector<FunctionNode*> getFunction(){
            return(this->functions);
        }

       
};

class AST{
    private:
        ProgramNode* root;
        std::vector<Token> Tokens;
    public:
        AST(ProgramNode* root){
            this->root =  root;
        }
        void PrettyPrint(){
            root->print();
        }
        
};