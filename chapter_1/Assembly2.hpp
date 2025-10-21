#ifndef IR_H
#define IR_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "AST.hpp"
#include "Tacky.hpp"

// ======================================================
//                     Operand Types
// ======================================================
enum OperandType { IMM, REG ,PSEUDO,STACK};

enum class RegisterName{AX,R10};

// ======================================================
//                     OperandNode(Base)
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
//                     ImmediateNode:OperandNode
// ======================================================
// Represents an immediate value (e.g., literal constants)
class ImmediateNode : public OperandNode {
    public:
        ImmediateNode(std::string val);

        std::string getImm(void);
        OperandType getType(void) override;
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;

    private:
        const std::string value;
};


// ======================================================
//                     RegisterNode:OperandNode
// ======================================================
// Represents a CPU register operand (e.g., eax, ebx)
class RegisterNode : public OperandNode {
    public:
        RegisterNode(RegisterName reg);

        RegisterName getRegEnum(void) const;
        std::string getRegStr(void) const;  // Convert enum to string
        OperandType getType(void) override;
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;

    private:
        const RegisterName reg;
};

// ======================================================
//                     Pseudo:OperandNode
// ======================================================
class Pseudo: public OperandNode{
    private:
        std::string identifier;
    
    public:
        Pseudo(std::string identifier);
        std::string getIdentifer();
        OperandType getType() override;
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;

};

// ======================================================
//                     Stack:OperandNode
// ======================================================
class Stack: public OperandNode{
    private:
        int amount;
    public:
        Stack(int amount);
        int getAmount();
        OperandType getType(void) override;
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;
};
// ======================================================
//                     Instruction Types
// ======================================================
enum InstructionType { MOV, RET,UNARY,ALLOCATE };


// ======================================================
//                     InstructionNode(Base)
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
//                     MoveInstruction:InstructionNode
// ======================================================
// Represents a MOV instruction (data transfer)
class MoveInstruction : public InstructionNode {
    public:
        MoveInstruction(OperandNode* s, OperandNode* d);

        OperandNode* getSrc(void);
        OperandNode* getDst(void);
        void filePrint(std::ofstream& assemblyFile) override;

    protected:
        void print() override;

    private:
        OperandNode* src;
        OperandNode* dst;
};


// ======================================================
//                     IRReturnNode:InstructionNode
// ======================================================
// Represents a RET instruction (function return)
class IRReturnNode : public InstructionNode {
    public:
        IRReturnNode();

        // std::string getReg();
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;

    private:
        // std::string reg;
};
// ======================================================
//                     Unary:InstructionNode
// ======================================================
class UnaryInstruction:public InstructionNode{
    private:
        UnaryOperator unary_operator;
        OperandNode* operand;
    public:
        UnaryInstruction(UnaryOperator unary_operator, OperandNode* operand);
        UnaryOperator getUnaryOperator();
        OperandNode* getOperand();
        void print() override;
        void filePrint(std::ofstream& assemblyFile) override;

};

// ======================================================
//                     AllocateStack:InstructionNode
// ======================================================
class AllocateStack: public InstructionNode{
    private:
        int amount;
    public:
        AllocateStack(int amount);
        int getAmount();
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
        
        
        std::string traverseTackyExpression(TackyVal* expression);
        std::vector<InstructionNode*> traverseTackyInstructions(std::vector<TackyInstruction*> instructions);
        std::vector<IRFunctionNode*> traverseTackyFunction( std::vector<TackyFunction*> functions);
       IRProgramNode* traverseTackyProgram( TackyProgram* program);

    public:
        IRTree(AST a);

        void transform();
        void prettyPrint();
        void filePrint(std::string assemblyFileName);

};

#endif
