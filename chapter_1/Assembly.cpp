#include "Assembly.hpp"
#include <iostream>
#include<limits.h>

// ======================================================
//                     OperandNode
// ======================================================

OperandNode::OperandNode(OperandType t)
    : type(t) {}

OperandNode::~OperandNode() {}


// ======================================================
//                     ImmediateNode::OperandNode
// ======================================================

ImmediateNode::ImmediateNode(std::string val) : OperandNode(IMM), value(val) {}

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

void ImmediateNode::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "Immediate(" << value << ")\n";
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
        case RegisterName::AX: return "eax";
        case RegisterName::R10: return "r10d";
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
void RegisterNode::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "Register(" << getRegStr() << ")\n";
}
// ======================================================
//                     Pseudo:OperandNode
// ======================================================
Pseudo::Pseudo(std::string identifier):OperandNode(PSEUDO), identifier(identifier){}
std::string Pseudo::getIdentifier(){
    return(this->identifier);
}

OperandType Pseudo::getType(){
    return(this->type);
}

void Pseudo::print(){
    std::cout<<identifier;
}

void Pseudo::filePrint(std::ofstream& assemblyFile){
    std::cout<<identifier;
}
void Pseudo::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "Pseudo(" << identifier << ")\n";
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

void Stack::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "Stack(offset=" << amount << ")\n";
}

// ======================================================
//                     InstructionNode(Base)
// ======================================================

InstructionNode::InstructionNode(InstructionType t)
    : type(t) {}

InstructionType InstructionNode::getType(){
    return(this->type);
}
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

void MoveInstruction::setSrc(OperandNode* newSrc){
    this->src =  newSrc;
}
void MoveInstruction::setDst(OperandNode* newDst){
    this->dst =  newDst;
}

void MoveInstruction::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "MoveInstruction(\n";
    indent(indentLevel + 1);
    std::cout << "src: ";
    src->prettyPrint(indentLevel + 2);
    indent(indentLevel + 1);
    std::cout << "dst: ";
    dst->prettyPrint(indentLevel + 2);
    indent(indentLevel);
    std::cout << ")\n";
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
    std::cout << "movq %rbp, %rsp\n";
    std::cout << "\tpopq %rbp\n";
    std::cout << "\tret\n";
    assemblyFile << "movq %rbp, %rsp\n";
    assemblyFile << "\tpopq %rbp\n";
    assemblyFile << "\tret\n";
}


void IRReturnNode::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "ReturnInstruction()\n";
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

void UnaryInstruction::setOperand(OperandNode* newOp){
    this->operand = newOp;
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

void UnaryInstruction::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "UnaryInstruction(op=";
    switch (unary_operator) {
        case UnaryOperator::Negation: std::cout << "Neg"; break;
        case UnaryOperator::Complement: std::cout << "Not"; break;
        default: std::cout << "Unknown"; break;
    }
    std::cout << ")\n";
    operand->prettyPrint(indentLevel + 1);
}
// ======================================================
//                     AllocateStack:InstructionNode
// ======================================================

AllocateStack::AllocateStack(int amount):InstructionNode(ALLOCATE), amount(amount){}

int AllocateStack::getStackDecrementAmount(){
    return(this->amount);
}

void AllocateStack::setStackDecrementAmount(int amount){
    this->amount = amount;
}
void AllocateStack::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "AllocateStack(bytes=" << amount << ")\n";
}

void AllocateStack::print(){
    std::cout<<"AllocateStack(bytes=" << amount << ")\n";
}

void AllocateStack::filePrint(std::ofstream& assemblyFile){
    std::cout<<"subq $"<<this->amount<<", %rsp\n";
    assemblyFile<<"subq $"<<this->amount<<", %rsp\n";
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

void IRFunctionNode::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "Function " << identifier << " {\n";
    for (auto* instr : instructions) {
        instr->prettyPrint(indentLevel + 1);
    }
    indent(indentLevel);
    std::cout << "}\n";
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
    // std::cout<<"HERE\n";
    for (IRFunctionNode* f : functions) {
        f->filePrint(assemblyFile);
        std::cout << '\n';
        assemblyFile<<'\n';
    }
    std::cout << ".section .note.GNU-stack,\"\",@progbits\n";
    assemblyFile<< ".section .note.GNU-stack,\"\",@progbits\n";
}


void IRProgramNode::prettyPrint(int indentLevel) const {
    indent(indentLevel);
    std::cout << "Program [\n";
    for (auto* fn : functions) {
        fn->prettyPrint(indentLevel + 1);
    }
    indent(indentLevel);
    std::cout << "]\n";
}
// ======================================================
//                     IRTree
// ======================================================

IRTree::IRTree(AST ast)
    : ast_root(ast), root(nullptr) {}
IRTree::IRTree(){}


std::vector<InstructionNode*> IRTree::traverseTackyInstructions(std::vector<TackyInstruction*> instructions){
    std::vector<InstructionNode*>  intermediateInstructions;
    intermediateInstructions.push_back(new AllocateStack{-1});
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



            if(TackyVariable* tackyVar = dynamic_cast<TackyVariable*>(dst)){
                dstOp = new Pseudo{tackyVar->getVariableIdentifier()};
            }else if(TackyConstant* tackyConst = dynamic_cast<TackyConstant*>(dst)){
                dstOp = new ImmediateNode{tackyConst->getValue()};
            }else{
                throw std::runtime_error("TESTING ERROR: dst is neither TackyVariable nor TackyConstant");
            }

            if(Pseudo* s1 = dynamic_cast<Pseudo*>(srcOp) ){
                if( Pseudo* d = dynamic_cast<Pseudo*>(dstOp)){
                        RegisterNode* movTmp = new RegisterNode{RegisterName::R10};
                        intermediateInstructions.push_back(new MoveInstruction(srcOp, movTmp));
                        intermediateInstructions.push_back(new MoveInstruction(movTmp, dstOp));
                        // MoveInstruction* mov = new MoveInstruction(srcOp, movTmp);

                }

            }else{

                MoveInstruction* mov = new MoveInstruction(srcOp, dstOp);
                intermediateInstructions.push_back(mov);
            }


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

std::vector<IRFunctionNode*> IRProgramNode::getFunctions(){
    return(this->functions);
}
// ======================================================
//                     IRTree
// ======================================================
void IRTree::transform() {
    // this->root = traverseProgram(ast_root.getRoot());

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


int& IRTree::getCurrentOffset(){
    return(this->currentOffset);
}

std::unordered_map<std::string, int>& IRTree::getPseudoOffsets(){
    return(this->pseudoOffsets);
}

void IRTree::replacePseudoOperands(){
    PseudoReplacer replacer{pseudoOffsets,currentOffset};
    std::unordered_set<Pseudo*> pseudoNodes;
    // std::cout<<"THERE are "<<root->getFunctions().size()<<" function(s)\n";
    int counter = 0;
    for(IRFunctionNode* f: root->getFunctions()){
        // std::cout<<f->getIdentifier()<<" has "<<f->getInstructions().size()<<" assembly instructions\n";
        for(InstructionNode* instr:f->getInstructions()){
            // std::cout<<"INSTRUCTION: "<<counter<<'\n';
            // std::cout<<"HERE\n";
            if(UnaryInstruction* unaryInstr =  dynamic_cast<UnaryInstruction*>(instr)){
                OperandNode* oldOperand =  unaryInstr->getOperand();
                // std::cout<<"UNARY"<<'\n';
                OperandNode* newOperand = replacer.replace(unaryInstr->getOperand(),pseudoNodes);
                // if(oldOperand == nullptr){
                //     std::cout<<"NULLPTR\n";
                // }
                if(oldOperand != newOperand){
                    unaryInstr->setOperand(newOperand);
                    // delete oldOperand;
                }
            }else if(MoveInstruction* movInstr =  dynamic_cast<MoveInstruction*>(instr)){
                // std::cout<<"MOVE"<<'\n';
                OperandNode* oldSrc =  movInstr->getSrc();
                OperandNode* oldDst = movInstr->getDst();

                OperandNode* newSrc  =  replacer.replace(movInstr->getSrc(),pseudoNodes);
                OperandNode* newDst = replacer.replace(movInstr->getDst(),pseudoNodes);

                if(oldSrc != newSrc){
                    movInstr->setSrc(newSrc);
                    // delete oldSrc;
                }
                if(oldDst != newDst){
                    movInstr->setDst(newDst);
                    // delete oldDst;
                }
                counter++;
            }
        }
        if(AllocateStack* allocate =  dynamic_cast<AllocateStack*>(f->getInstructions()[0])){
            allocate->setStackDecrementAmount(currentOffset+4);
        }

        replacer.resetOffsets();
    }
    for(Pseudo* p: pseudoNodes){
        delete p;
    }
}

IRProgramNode* IRTree::transformFromTacky(TackyProgram* tackyProgram){
    this->root = traverseTackyProgram(tackyProgram);
    return(this->root);
}

PseudoReplacer::PseudoReplacer(std::unordered_map<std::string,int>& offsets, int& currentFreeOffset)
    : offsets(offsets), currentFreeOffset(currentFreeOffset) {}

OperandNode* PseudoReplacer::replace(OperandNode* op,std::unordered_set<Pseudo*>& pseudoNodes){
    // std::cout<<"IN REPLACE\n";
    
    if(Pseudo* pseudo = dynamic_cast<Pseudo*>(op)){
        // std::cout<<"IS PSUEDO NODE\n";
        std::string name = pseudo->getIdentifier();
        if(offsets.find(name) ==  offsets.end()){
            offsets[pseudo->getIdentifier()] =  currentFreeOffset;
            if(currentFreeOffset  < INT_MIN +4){
                throw std::runtime_error("Cannot decrement offset anymore");
            }
            currentFreeOffset-=4;
        }
        pseudoNodes.insert(pseudo);
        Stack* stack =  new Stack{offsets[name]};
        // std::cout<<"REPLACED PSEUDO NODE WITH STACK ADDRESS\n";
        return(stack);
    }
    // std::cout<<"EXITING REPLACE DID NOT MAKE CHANGE\n";
    return(op);
}

void PseudoReplacer::resetOffsets(){
    this->offsets.clear();
    this->currentFreeOffset = -4;
}
