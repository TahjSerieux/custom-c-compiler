#include "Tacky.hpp"


// ======================================================
//                     TackyConstant
// ======================================================
TackyConstant::TackyConstant(std::string val):value(val){}

std::string TackyConstant::getValue(){
    return(this->value);
}

TackyConstant::~TackyConstant(){}

void TackyConstant::print() const {
    std::cout << this->value;
}
// ======================================================
//                     TackyVariable
// ======================================================
TackyVariable::TackyVariable(std::string varIdentifier):variableIdentifier(varIdentifier){}

std::string TackyVariable::getVariableIdentifier(){
    return(this->variableIdentifier);
}

void TackyVariable::print() const {
    std::cout << this->variableIdentifier;
}
// ======================================================
//                     TackyReturn
// ======================================================

TackyReturn::TackyReturn(TackyVal* var):var(var){}

TackyVal* TackyReturn::getVar(){
    return(this->var);
}

void TackyReturn::print() const {
    std::cout << "return ";
    if (this->var) this->var->print();
    std::cout << ";\n";
}
// ======================================================
//                     TackyUnary
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

void TackyFunction::print() const {
    std::cout << "function " << identifier << "():\n";
    for (auto* instr : body) {
        instr->print();
    }
    std::cout << "\n";
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
    for (auto* func : functions) {
        func->print();
    }
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
std::vector<TackyInstruction*> TackyGenerator::convertInstructions(StatementNode* statement){
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
    std::vector<TackyInstruction*> instructions = convertInstructions(function->getStatement());
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