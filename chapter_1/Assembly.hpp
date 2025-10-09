#ifndef IR_H
#define IR_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "AST.hpp"

// ======================================================
//                     Operand Types
// ======================================================
enum OperandType { IMM, REG };


// ======================================================
//                     OperandNode
// ======================================================
// Abstract base class for all operand types (immediates, registers, etc.)
class OperandNode {
    public:
        virtual ~OperandNode() = 0;

        virtual OperandType getType(void) = 0;
        virtual void print() = 0;
        virtual void filePrint(std::ofstream& assemblyFile) = 0;

    protected:
        OperandNode(OperandType t);
        OperandType type;
};


// ======================================================
//                     ImmediateNode
// ======================================================
// Represents an immediate value (e.g., literal constants)
class ImmediateNode : public OperandNode {
    public:
        ImmediateNode(std::string val);

        std::string getImm(void);
        OperandType getType(void);
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;

    private:
        const std::string value;
};


// ======================================================
//                     RegisterNode
// ======================================================
// Represents a CPU register operand (e.g., eax, ebx)
class RegisterNode : public OperandNode {
    public:
        RegisterNode(std::string reg);

        std::string getReg(void);
        OperandType getType(void);
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;

    private:
        const std::string reg;
};


// ======================================================
//                     Instruction Types
// ======================================================
enum InstructionType { MOV, RET };


// ======================================================
//                     InstructionNode
// ======================================================
// Abstract base class for all assembly-level instructions
class InstructionNode {
    public:
        virtual void print() = 0;
        virtual void filePrint(std::ofstream& assemblyFile) = 0;

    protected:
        InstructionNode(InstructionType t);
        InstructionType type;
};


// ======================================================
//                     MoveInstruction
// ======================================================
// Represents a MOV instruction (data transfer)
class MoveInstruction : public InstructionNode {
    public:
        MoveInstruction(ImmediateNode* s, OperandNode* d);

        ImmediateNode* getSrc(void);
        OperandNode* getDst(void);
        void filePrint(std::ofstream& assemblyFile) override;

    protected:
        void print() override;

    private:
        ImmediateNode* src;
        OperandNode* dst;
};


// ======================================================
//                     IRReturnNode
// ======================================================
// Represents a RET instruction (function return)
class IRReturnNode : public InstructionNode {
    public:
        IRReturnNode(std::string reg);

        std::string getReg();
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;

    private:
        std::string reg;
};


// ======================================================
//                     IRFunctionNode
// ======================================================
// Represents a function at the IR (intermediate representation) level
class IRFunctionNode {
    public:
        IRFunctionNode(std::string identifier, std::vector<InstructionNode*> instr);

        const std::string getIdentifier(void);
        std::vector<InstructionNode*> getInstructions(void);

        void print();
        void filePrint(std::ofstream& assemblyFile);

    private:
        std::string identifier;
        std::vector<InstructionNode*> instructions;
};


// ======================================================
//                     IRProgramNode
// ======================================================
// Represents an entire IR program (collection of functions)
class IRProgramNode {
    public:
        IRProgramNode(std::vector<IRFunctionNode*> fs);

        void print();
        void filePrint(std::ofstream& assemblyFile);

    private:
        std::vector<IRFunctionNode*> functions;
};


// ======================================================
//                     IRTree
// ======================================================
// Transforms an AST into an IR tree representation
class IRTree {
    private:
        AST ast_root;
        IRProgramNode* root;
        std::ofstream assemblyFile;
        std::string traverseExpression(ExpressionNode* expression);
        std::vector<InstructionNode*> traverseStatement(StatementNode* statement);
        std::vector<IRFunctionNode*> traverseFunction(const std::vector<FunctionNode*> functions);
        IRProgramNode* traverseProgram(const ProgramNode* program);

    public:
        IRTree(AST a);

        void transform();
        void prettyPrint();
        void filePrint(std::string assemblyFileName);
};

#endif
