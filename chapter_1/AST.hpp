#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <iostream>
enum ASTNodeType{program,function_definition,satement,exp};

class ExpNode{
    public:
        ExpNode(std::string value){
            this->constant_expression = value;
        }
        std::string getValue(){
            return(this->constant_expression);
        }
        void print(){
            std::cout<<"\t\t\t Constant("<<constant_expression<<")";
        }

    private:
        std::string constant_expression;



};
class StatementNode{
    public:
        StatementNode( ExpNode * return_exp){
            this->exp =  return_exp;
        }

        const ExpNode* get_exp(){
            return(this->exp);
        }
        void print(){
            std::cout<<"Return(\n";
            exp->print();
            std::cout<<"\n\t\t)\n";
        }

    private:
        ExpNode* exp;

};
class IdentifierNode{
    public:
        IdentifierNode(std::string str){
            this->identifier =  str;
        }
        std::string getIdentifier(){
            return(this->identifier);
        }
    private:
        std::string identifier;


};
class ReturnStatement: StatementNode{
    public:

    private:

};
class IfStatement : StatementNode{
    public:
    private:
};
class FunctionDefinitionNode{
    public:
        FunctionDefinitionNode(std::string name, StatementNode* function_body){
            this->name =  name;
            this->function_body = function_body;

        }

        const std::string get_name(){
            return(this->name);
        }
        const StatementNode* get_body(){
            return(this->function_body);
        }
        void print(){
            std::cout<<"\tFunction(\n\t\t";
            std::cout<<"name=\""<<name<<"\"\n";
            std::cout<< "\t\tbody=";function_body->print();
            std::cout<<"\t)\n";
            // std::cout<<'\n';
        }
        
    private:
        std::string name;
        StatementNode* function_body;

};

class ProgramNode{
    public:
        ProgramNode(FunctionDefinitionNode* func){
            this->function_definitions.push_back(func);
        }
        ProgramNode(std::vector<FunctionDefinitionNode*>& funcs){
            this->function_definitions= funcs;
        }
        ProgramNode(){
            this->function_definitions = {};
        }
        void setFunctions(std::vector<FunctionDefinitionNode*>& v){
            this->function_definitions = v;
        }
        const std::vector<FunctionDefinitionNode*> get_function(){
            return(this->function_definitions);
        }
        void print(){
            // std::cout<<"Program(\n\t";
            // function_definitions->print();
            for(FunctionDefinitionNode* f : function_definitions){
                f->print();
            }
            // std::cout<<")\n";
        }
    private:
        std::vector<FunctionDefinitionNode*> function_definitions;


};

#endif // AST_HPP
