#include "Assembly.hpp"
#include <iostream>

// ======================================================
//                     OperandNode
// ======================================================

OperandNode::OperandNode(OperandType t)
    : type(t) {}

OperandNode::~OperandNode() {}


// ======================================================
//                     ImmediateNode
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
//                     RegisterNode
// ======================================================

RegisterNode::RegisterNode(std::string reg)
    : OperandNode(REG), reg(reg) {}

std::string RegisterNode::getReg(void) {
    return this->reg;
}

OperandType RegisterNode::getType(void) {
    return this->type;
}

void RegisterNode::print() {
    std::cout << reg;
}

void RegisterNode::filePrint(std::ofstream& assemblyFile) {
    std::cout << "%" << reg;
    assemblyFile<<"%"<<reg;

}


// ======================================================
//                     InstructionNode
// ======================================================

InstructionNode::InstructionNode(InstructionType t)
    : type(t) {}


// ======================================================
//                     MoveInstruction
// ======================================================

MoveInstruction::MoveInstruction(ImmediateNode* s, OperandNode* d)
    : InstructionNode(MOV), src(s), dst(d) {}

ImmediateNode* MoveInstruction::getSrc(void) {
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
    // std::cout << "movl ";
    assemblyFile << "movl ";
    src->filePrint(assemblyFile);
    // std::cout << ", ";
    assemblyFile << ", ";
    dst->filePrint(assemblyFile);
    // std::cout << '\n';
    assemblyFile << '\n';
}


// ======================================================
//                     IRReturnNode
// ======================================================

IRReturnNode::IRReturnNode(std::string reg)
    : InstructionNode(RET), reg(reg) {}

std::string IRReturnNode::getReg(void) {
    return this->reg;
}

void IRReturnNode::print() {
    std::cout << "\t\t\tReturn(" << reg << ")\n";
}

void IRReturnNode::filePrint(std::ofstream& assemblyFile) {
    std::cout << "ret\n";
    assemblyFile << "ret\n";
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
    if (expression->getType() == CONSTANT) {
        ConstantNode* constNode = dynamic_cast<ConstantNode*>(expression);
        return constNode->getValue();
    }
    return "";
}

std::vector<InstructionNode*> IRTree::traverseStatement(StatementNode* statement) {
    std::vector<InstructionNode*> instructions;

    if (statement->getType() == RETURN) {
        ReturnNode* returnNode = dynamic_cast<ReturnNode*>(statement);
        std::string value = traverseExpression(returnNode->getExpression());

        ImmediateNode* immNode = new ImmediateNode(value);
        RegisterNode* regNode = new RegisterNode("eax");
        MoveInstruction* movInstruction = new MoveInstruction(immNode, regNode);
        IRReturnNode* retInstruction = new IRReturnNode("eax");

        instructions.push_back(movInstruction);
        instructions.push_back(retInstruction);
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

void IRTree::transform() {
    this->root = traverseProgram(ast_root.getRoot());
}

void IRTree::prettyPrint() {
    std::cout << "------ASSEMBLY------\n";
    root->print();
}

void IRTree::filePrint(std::string assemblyFileName) {
    // assemblyFile.open("assemblyFileName + "".s");
    assemblyFile.open("Assembly.s");
    root->filePrint(assemblyFile);
    assemblyFile.close();
}
