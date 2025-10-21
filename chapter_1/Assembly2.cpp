#include "Assembly2.hpp"
#include <iostream>

// ======================================================
//                     OperandNode
// ======================================================

OperandNode::OperandNode(OperandType t)
    : type(t) {}

OperandNode::~OperandNode() {}


// ======================================================
//                     ImmediateNode::OperandNode
// ======================================================

ImmediateNode::ImmediateNode(std::string val)
    : OperandNode(IMM), value(val) {}

std::string ImmediateNode::getImm(void) {
    return this->value;
}

OperandType ImmediateNode::getType(void) {
    return this->type;
}

void ImmediateNode::print() {
    std::cout << value;
}

void ImmediateNode::filePrint(std::ofstream& assemblyFile) {
    std::cout << "$" << value;
    assemblyFile << "$" << value;
}


// ======================================================
//                     RegisterNode:OperandNode
// ======================================================

RegisterNode::RegisterNode(RegisterName reg): OperandNode(REG), reg(reg) {}

RegisterName RegisterNode::getRegEnum(void) const {
    return this->reg;
}

std::string RegisterNode::getRegStr(void) const{
    switch (this->reg){
        case RegisterName::AX: return "AX";
        case RegisterName::R10: return "R10";
        default: return "UNKOWN";
    }

}
OperandType RegisterNode::getType(void) {
    return this->type;
}

void RegisterNode::print() {
    std::cout << getRegStr();
}

void RegisterNode::filePrint(std::ofstream& assemblyFile) {
    std::cout << "%" << getRegStr();
    assemblyFile<<"%"<<getRegStr();

}

// ======================================================
//                     Pseudo:OperandNode
// ======================================================
Pseudo::Pseudo(std::string identifier):OperandNode(PSEUDO), identifier(identifier){}
std::string Pseudo::getIdentifer(){
    return(this->identifier);
}

OperandType Pseudo::getType(){
    return(this->type);
}

void Pseudo::print(){

}

void Pseudo::filePrint(std::ofstream& assemblyFile){

}

// ======================================================
//                     Stack:OperandNode
// ======================================================

Stack::Stack(int amount):OperandNode(STACK), amount(amount){}

int Stack::getAmount(){
    return this->amount;
}

OperandType Stack::getType(void){
    return this->type;
}

void Stack::print(){
    std::cout << amount << "(%rbp)";
}

void Stack::filePrint(std::ofstream& assemblyFile){
    std::cout << amount << "(%rbp)";
    assemblyFile << amount << "(%rbp)";
}
// ======================================================
//                     InstructionNode(Base)
// ======================================================

InstructionNode::InstructionNode(InstructionType t)
    : type(t) {}


// ======================================================
//                     MoveInstruction:InstructionNode
// ======================================================

MoveInstruction::MoveInstruction(OperandNode* s, OperandNode* d)
    : InstructionNode(MOV), src(s), dst(d) {}

OperandNode* MoveInstruction::getSrc(void) {
    return this->src;
}

OperandNode* MoveInstruction::getDst(void) {
    return this->dst;
}

void MoveInstruction::print() {
    std::cout << "\t\t\tMov(";
    src->print();
    std::cout << ", ";
    dst->print();
    std::cout << ")\n";
}

void MoveInstruction::filePrint(std::ofstream& assemblyFile) {
    std::cout << "movl ";
    assemblyFile << "movl ";
    src->filePrint(assemblyFile);
    std::cout << ", ";
    assemblyFile << ", ";
    dst->filePrint(assemblyFile);
    std::cout << '\n';
    assemblyFile << '\n';
}


// ======================================================
//                     IRReturnNode:InstructionNode
// ======================================================

IRReturnNode::IRReturnNode()
    : InstructionNode(RET){}

// std::string IRReturnNode::getReg(void) {
//     return this->reg;
// }

void IRReturnNode::print() {
    std::cout << "\t\t\tret\n";
}

void IRReturnNode::filePrint(std::ofstream& assemblyFile) {
    std::cout << "ret\n";
    assemblyFile << "ret\n";
}

// ======================================================
//                     Unary:InstructionNode
// ======================================================

UnaryInstruction::UnaryInstruction(UnaryOperator unary_operator, OperandNode* operand):InstructionNode(UNARY),unary_operator(unary_operator), operand(operand){}
UnaryOperator UnaryInstruction::getUnaryOperator(){
    return(this->unary_operator);
}
OperandNode* UnaryInstruction::getOperand(){
    return(this->operand);
}

void UnaryInstruction::print(){
    std::string opStr;
    switch (unary_operator) {
        case UnaryOperator::Complement:
            opStr = "notl";
            break;
        case UnaryOperator::Negation:
            opStr = "negl";
            break;
        default:
            opStr = "unknown_unary";
    }

    std::cout << opStr << " ";
    operand->print();
    std::cout << "\n";

}
void UnaryInstruction::filePrint(std::ofstream& assemblyFile) {
    std::string opStr;
    switch (unary_operator) {
        case UnaryOperator::Complement:
            opStr = "notl";
            break;
        case UnaryOperator::Negation:
            opStr = "negl";
            break;
        default:
            opStr = "unknown_unary";
    }

    std::cout << opStr << " ";
    assemblyFile << opStr << " ";

    operand->filePrint(assemblyFile);

    std::cout << "\n";
    assemblyFile << "\n";
}
// ======================================================
//                     AllocateStack:InstructionNode
// ======================================================

AllocateStack::AllocateStack(int amount):InstructionNode(ALLOCATE), amount(amount){}

int AllocateStack::getAmount(){
    return(this->amount);
}
// ======================================================
//                     IRFunctionNode
// ======================================================

IRFunctionNode::IRFunctionNode(std::string identifier, std::vector<InstructionNode*> instr)
    : identifier(identifier), instructions(instr) {}

const std::string IRFunctionNode::getIdentifier(void) {
    return this->identifier;
}

std::vector<InstructionNode*> IRFunctionNode::getInstructions(void) {
    return this->instructions;
}

void IRFunctionNode::print() {
    std::cout << "\tFunction(\n";
    std::cout << "\t\tname=" << this->identifier << '\n';
    std::cout << "\t\tbody={\n";
    for (InstructionNode* i : instructions) {
        i->print();
    }
    std::cout << "\t\t}\n";
    std::cout << "\t)\n";
}

void IRFunctionNode::filePrint(std::ofstream& assemblyFile) {
    std::cout << "\t.global " << identifier << "\n";
    assemblyFile << "\t.global " << identifier << "\n";
    std::cout << identifier << ":\n";
    assemblyFile<< identifier << ":\n";
    for (InstructionNode* i : instructions) {
        assemblyFile << "\t";
        std::cout << "\t";
        i->filePrint(assemblyFile);
    }
}


// ======================================================
//                     IRProgramNode
// ======================================================

IRProgramNode::IRProgramNode(std::vector<IRFunctionNode*> fs)
    : functions(fs) {}

void IRProgramNode::print() {
    std::cout << "Program(\n";
    for (IRFunctionNode* f : this->functions) {
        f->print();
        std::cout << "\n";
    }
    std::cout << ")\n";
}

void IRProgramNode::filePrint(std::ofstream& assemblyFile) {
    for (IRFunctionNode* f : functions) {
        f->filePrint(assemblyFile);
        std::cout << '\n';
        assemblyFile<<'\n';
    }
    std::cout << ".section .note.GNU-stack,\"\",@progbits\n";
    assemblyFile<< ".section .note.GNU-stack,\"\",@progbits\n";
}


// ======================================================
//                     IRTree
// ======================================================

IRTree::IRTree(AST ast)
    : ast_root(ast), root(nullptr) {}

std::string IRTree::traverseExpression(ExpressionNode* expression) {
    if (expression->getType() == ExpressionType::CONSTANT) {
        ConstantNode* constNode = dynamic_cast<ConstantNode*>(expression);
        return constNode->getValue();
    }
    return "";
}

std::vector<InstructionNode*> IRTree::traverseStatement(StatementNode* statement) {
    std::vector<InstructionNode*> instructions;

    if (statement->getType() == StatementType::RETURN) {
        ReturnNode* returnNode = dynamic_cast<ReturnNode*>(statement);
        std::string value = traverseExpression(returnNode->getExpression());

        ImmediateNode* immNode = new ImmediateNode(value);
        RegisterNode* regNode = new RegisterNode(RegisterName::R10);
        MoveInstruction* movInstruction = new MoveInstruction(immNode, regNode);
        // IRReturnNode* retInstruction = new IRReturnNode("eax");

        instructions.push_back(movInstruction);
        // instructions.push_back(retInstruction);
    }

    return instructions;
}

std::vector<IRFunctionNode*> IRTree::traverseFunction(const std::vector<FunctionNode*> functions) {
    std::vector<IRFunctionNode*> programFunctions;

    for (FunctionNode* f : functions) {
        std::string id = f->getIdentifer();
        std::vector<InstructionNode*> instructs = traverseStatement(f->getStatement());
        programFunctions.push_back(new IRFunctionNode(id, instructs));
    }

    return programFunctions;
}

IRProgramNode* IRTree::traverseProgram(const ProgramNode* program) {
    return new IRProgramNode(this->traverseFunction(program->getFunction()));
}


std::vector<InstructionNode*> IRTree::traverseTackyInstructions(std::vector<TackyInstruction*> instructions){
    std::vector<InstructionNode*>  intermediateInstructions;
    for(TackyInstruction* instr: instructions){
        if(TackyReturn* tackyReturn = dynamic_cast<TackyReturn*>(instr)){
            if(TackyConstant* tackyConstant =  dynamic_cast<TackyConstant*>(tackyReturn->getVar())){
                ImmediateNode* imm = new ImmediateNode{tackyConstant->getValue()};
                RegisterNode* reg = new RegisterNode{RegisterName::AX};
                MoveInstruction* mov = new MoveInstruction{imm,reg};
                intermediateInstructions.push_back(mov);
                IRReturnNode* ret =  new IRReturnNode{};
                intermediateInstructions.push_back(ret);
            }else if(TackyVariable* tackyVar = dynamic_cast<TackyVariable*>(tackyReturn->getVar())){
                Pseudo* psuedo = new Pseudo{tackyVar->getVariableIdentifier()};
                RegisterNode* reg = new RegisterNode{RegisterName::AX};
                MoveInstruction* mov = new MoveInstruction{psuedo,reg};
                intermediateInstructions.push_back(mov);
                IRReturnNode* ret =  new IRReturnNode{};
                intermediateInstructions.push_back(ret);
            }

        }else if(TackyUnary* tackyUnary = dynamic_cast<TackyUnary*>(instr)){
            UnaryOperator unaryOperator = tackyUnary->getUnaryOperator();
            TackyVal* src = tackyUnary->getSrc();
            TackyVal* dst = tackyUnary->getDst();

            OperandNode* srcOp = nullptr;
            OperandNode* dstOp = nullptr;
            
            if(TackyConstant* tackyConst =  dynamic_cast<TackyConstant*>(src)){
                srcOp = new ImmediateNode{tackyConst->getValue()};
            }else if(TackyVariable* tackyVar = dynamic_cast<TackyVariable*>(src)){
                srcOp = new Pseudo{tackyVar->getVariableIdentifier()};
            }

            if(RegisterNode* regNode = dynamic_cast<RegisterNode*>(dst)){
                dstOp =  new Pseudo{regNode->getRegStr()};
            }

            MoveInstruction* mov = new MoveInstruction(srcOp, dstOp);
            intermediateInstructions.push_back(mov);

            UnaryInstruction* unaryInstr = new UnaryInstruction{unaryOperator,dstOp};
            intermediateInstructions.push_back(unaryInstr);

        }   
    }
    return(intermediateInstructions);
}

std::vector<IRFunctionNode*> IRTree::traverseTackyFunction(std::vector<TackyFunction*> functions){
    std::vector<IRFunctionNode*> programFunctions;
    for(TackyFunction* f: functions){
        std::string identifer =  f->getIdentifier();
        std::vector<InstructionNode*> instructions = traverseTackyInstructions(f->getBody());
        programFunctions.push_back(new IRFunctionNode{identifer,instructions});
    }
    return(programFunctions);
}
IRProgramNode* IRTree::traverseTackyProgram( TackyProgram* program){
    return(new IRProgramNode{traverseTackyFunction(program->getFunctions())});
}

void IRTree::transform() {
    this->root = traverseProgram(ast_root.getRoot());
}

void IRTree::prettyPrint() {
    std::cout << "------ASSEMBLY------\n";
    root->print();
}

void IRTree::filePrint(std::string assemblyFileName) {
    // assemblyFile.open("assemblyFileName + "".s");
    assemblyFile.open("../Assembly.s");
    root->filePrint(assemblyFile);
    assemblyFile.close();
}
