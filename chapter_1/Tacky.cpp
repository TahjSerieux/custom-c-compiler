#include "Tacky.hpp"

// Helper for indentation
static void printIndent(int indent) {
    std::cout << std::string(indent * 2, ' ');
}


// ======================================================
//                     TackyConstant:TackyVal
// ======================================================
TackyConstant::TackyConstant(std::string val):value(val){}

std::string TackyConstant::getValue(){
    return(this->value);
}

TackyConstant::~TackyConstant(){}

void TackyConstant::print() const {
    std::cout << this->value;
}

void TackyConstant::prettyPrint(int indent) const {
    printIndent(indent);
    std::cout << "Constant(" << this->value << ")\n";
}

// ======================================================
//                     TackyVariable:TackyVal
// ======================================================
TackyVariable::TackyVariable(std::string varIdentifier):variableIdentifier(varIdentifier){}

TackyVariable::~TackyVariable(){}

std::string TackyVariable::getVariableIdentifier(){
    return(this->variableIdentifier);
}



void TackyVariable::print() const {
    std::cout << this->variableIdentifier;
}

void TackyVariable::prettyPrint(int indent) const {
    printIndent(indent);
    std::cout << "Variable(" << this->variableIdentifier << ")\n";
}

// ======================================================
//                     TackyReturn:TackyInstruction
// ======================================================

TackyReturn::TackyReturn(TackyVal* var):val(var){}

TackyReturn::~TackyReturn(){}

TackyVal* TackyReturn::getVar(){
    return(this->val);
}



void TackyReturn::print() const {
    std::cout << "return ";
    if (this->val) this->val->print();
    std::cout << ";\n";
}

void TackyReturn::prettyPrint(int indent) const {
    printIndent(indent);
    std::cout << "Return:\n";
    if (this->val) this->val->prettyPrint(indent + 1);
}
// ======================================================
//                     TackyUnary:TackyInstruction
// ======================================================
TackyUnary::TackyUnary(UnaryOperator unary_operator,TackyVal* src,TackyVal* dst):
    unary_operator(unary_operator),
    src(src),
    dst(dst){}

TackyUnary::~TackyUnary(){}
UnaryOperator TackyUnary::getUnaryOperator(){
    return(this->unary_operator);
}

TackyVal* TackyUnary::getSrc(){
    return(this->src);
}

TackyVal* TackyUnary::getDst(){
    return(this->dst);
}

void TackyUnary::print() const {
    std::cout << "  ";
    if (this->dst) this->dst->print();
    std::cout << " = " << unary_operator_to_string(this->unary_operator);
    if (this->src) {
        std::cout << " ";
        this->src->print();
    }
    std::cout << ";\n";
}

void TackyUnary::prettyPrint(int indent) const {
    printIndent(indent);
    std::cout << "Unary(" << unary_operator_to_string(this->unary_operator) << "):\n";
    printIndent(indent + 1);
    std::cout << "Dst -> ";
    if (this->dst) this->dst->prettyPrint(0); else std::cout << "None\n";
    printIndent(indent + 1);
    std::cout << "Src -> ";
    if (this->src) this->src->prettyPrint(0); else std::cout << "None\n";
}

// ======================================================
//                     TackyFunction
// ======================================================

TackyFunction::TackyFunction(std::string identifier, std::vector<TackyInstruction*> body):identifier(identifier), body(body){}

std::string TackyFunction::getIdentifier(){
    return(this->identifier);
}

std::vector<TackyInstruction*> TackyFunction::getBody(){
    return(this->body);
}

// void TackyFunction::print() const {
//     std::cout << "function " << identifier << "():\n";
//     for (auto* instr : body) {
//         instr->print();
//     }
//     std::cout << "\n";
// }

void TackyFunction::print() const {
    std::cout << "function " << identifier << "():\n";
    for (auto* instr : body)
        instr->print();
    std::cout << "\n";
}

void TackyFunction::prettyPrint(int indent) const {
    printIndent(indent);
    std::cout << "Function " << identifier << "():\n";
    for (auto* instr : body)
        instr->prettyPrint(indent + 1);
}
// ======================================================
//                     TackyProgram
// ======================================================
TackyProgram::TackyProgram(std::vector<TackyFunction*> functions):functions(functions){}

std::vector<TackyFunction*> TackyProgram::getFunctions(){
    return(this->functions);
}



void TackyProgram::print() const {
    std::cout << "=== TAC Program ===\n";
    for (auto* func : functions)
        func->print();
}

void TackyProgram::prettyPrint(int indent) const {
    printIndent(indent);
    std::cout << "TAC Program:\n";
    for (auto* func : functions)
        func->prettyPrint(indent + 1);
}

// ======================================================
//                     TackyGenerator
// ======================================================
TackyVal*  TackyGenerator::convertExpression(ExpressionNode* expression,std::vector<TackyInstruction*>& instructions){
    
    ExpressionType type =  expression->getType();
    if(type == ExpressionType::CONSTANT){
        ConstantNode* constantNode = dynamic_cast<ConstantNode*>(expression);
        return (new TackyConstant{constantNode->getValue()});
        // TackyReturn* tackyReturn =  new TackyReturn{tackyConstant};
        // instructions.push_back(tackyConstant);
    }else if(type ==  ExpressionType::UNARY){
        UnaryNode* unaryNode =  dynamic_cast<UnaryNode*>(expression);
        UnaryOperator unaryOperator = unaryNode->get_unary_operator();
        TackyVal* src = convertExpression(unaryNode->getExpression(), instructions);
        
        TackyVariable* dst = new TackyVariable{this->make_temporary()};
        TackyUnary* inst =  new TackyUnary{unaryOperator,src,dst};
        instructions.push_back(inst);
        return dst;
        
    }
    return(nullptr);
}

std::vector<TackyInstruction*> TackyGenerator::convertStatement(StatementNode* statement){
    std::vector<TackyInstruction*> instructions;
    StatementType type = statement->getType();
    if (type == StatementType::RETURN) {
        ReturnNode* returnNode = dynamic_cast<ReturnNode*>(statement);
        TackyVal* val = convertExpression(returnNode->getExpression(), instructions);
        TackyReturn* tackyReturn = new TackyReturn{val};
        instructions.push_back(tackyReturn);
    }
    return(instructions);
}

TackyFunction* TackyGenerator::convertFunction(FunctionNode* function){
    std::string identifier =  function->getIdentifer();
    std::vector<TackyInstruction*> instructions = convertStatement(function->getStatement());
    return(new TackyFunction{identifier,instructions});
}

TackyProgram* TackyGenerator::convertProgram(AST* ast){
    std::vector<TackyFunction*> tackyFunctions;
    for(FunctionNode* f: ast->getRoot()->getFunction()){
        tackyFunctions.push_back(convertFunction(f));
    }
    return(new TackyProgram{tackyFunctions});
}
TackyGenerator::TackyGenerator():temp_counter(0){}

std::string TackyGenerator::make_temporary(){
    return( "tmp."+std::to_string(this->temp_counter++));
}

/*
Lexer: goes throgh the source file and construct Tokens based on the sybmols it sees.
-------
The lexer (or tokenizer) scans the raw source code and groups sequences of characters into meaningful symbols called tokens.
Each token has a type (like IDENTIFIER, NUMBER, PLUS, IF, RETURN, etc.) and a value (like "x" or "42").

➜ Purpose: Turn a raw text stream into a structured list of symbols that the parser can reason about.
➜ Output: A list (or stream) of Token objects.


Parser: Traverses the Tokens fenerted by the Lexer. The parser acts upon the grammar
of the language and makes sure that the order of the Token is in line with the syntax
of the language. The parser outputs a abstract syntax tree which si the contents of
the source code but abstracted away with the organizational and heriarichal symbols
removed. This is because structure of the AST(Abstract Synta Tree) already conveys
the organizational & heriarichal strucutre of the source code.
--------
The parser takes the token stream from the lexer and applies the grammar rules of the language to verify that the sequence is valid and to build a hierarchical representation — the Abstract Syntax Tree (AST).

➜ Purpose: Enforce syntax and express structure.
➜ Output: An AST that represents what the program means, not just what it looks like.


Tacky: Complex/Nested Expression cannot be easily translated to assembly. 
Instead of having one long chain of nested expressions, we represent each expression in the form of
TAC(Three Operand Coding), where each expression has at most three Operands.
-(~(-3))=> tmp.0 = -3; tmp1= ~tmp.0; tmp2=-tmp.1.
This example the unary operator has two operands, the src and dst, 
the unary operator denoting what operation to perform on the src value. This also has the added benefit
of being closer to what the assembly code looks like.
*/