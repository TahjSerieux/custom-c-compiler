#include "Tacky.hpp"


// ======================================================
//                     TackyProgram
// ======================================================
TackyProgram::TackyProgram(std::vector<TackyFunction*> functions):functions(functions){}

std::vector<TackyFunction*> TackyProgram::getFunctions(){
    return(this->functions);
}
// ======================================================
//                     TackyGenerator
// ======================================================
void TackyGenerator::convertExpression(ExpressionNode* expression,std::vector<TackyInstruction*>& instructions){
    
    ExpressionType type =  expression->getType();
    if(type == ExpressionType::CONSTANT){
        TackyConstant* node = new TackyConstant{expression->getValue()};

    }else if(type ==  ExpressionType::UNARY){

    }
}
std::vector<TackyInstruction*> TackyGenerator::convertInstructions(StatementNode* statement){
    std::vector<TackyInstruction*> instructions;
    if (statement->getType() == StatementType::RETURN) {
        ReturnNode* returnNode = dynamic_cast<ReturnNode*>(statement);
        
    }
    return(instructions);
}
TackyFunction* TackyGenerator::convertFunction(FunctionNode* function){
    std::string identifier =  function->getIdentifer();

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