#ifndef IR_H
#define  IR_H
#include<string>
#include<vector>
#include<iostream>
#include "AST.hpp"
//ASTRACT DEFINITIONS
//Operand type nodes
enum OperandType{IMM,REG};
class OperandNode{
    public:
        virtual ~OperandNode() = 0;
        virtual OperandType getType(void) = 0;
        virtual void print() = 0;
        virtual void filePrint() = 0;
    private:
    protected:
        OperandNode(OperandType t);
        
        OperandType type;
    };
    //Immediate Values
    class ImmediateNode: public OperandNode{
        public:
            ImmediateNode(std::string val);
            std::string getImm(void);
            OperandType getType(void);
            void print() override;
            void filePrint() override;
            private:
            const std::string value;
            protected:
        };
        //Registers
        class RegisterNode: public OperandNode{
            public:
            RegisterNode(std::string reg);
            std::string getReg(void);
            OperandType getType(void);
            void print() override;
            void filePrint() override;
        private:
            const std::string reg;

        protected:
    };
enum InstructionType{MOV,RET};
//Instruction type nodes
class InstructionNode{
    public:
        virtual void print()=0;
        virtual void filePrint()=0;
    private:
    protected:
        InstructionNode(InstructionType t);
        InstructionType type;
};
    class MoveInstruction: public InstructionNode{
        public:
            MoveInstruction(ImmediateNode* s,OperandNode* d);
            ImmediateNode* getSrc(void);
            OperandNode* getDst(void);
            void filePrint() override;
        private:
            ImmediateNode* src;
            OperandNode* dst;
        protected:
            void print() override;
        };
    class IRReturnNode: public InstructionNode{
        public:
            IRReturnNode(std::string reg);
            std::string getReg();
            void print() override;
            void filePrint() override;
        private:
            std::string reg;
        protected:
    };

//CONCRETE DEFINITIONS
class IRFunctionNode{
    public:
        IRFunctionNode(std::string identifier, std::vector<InstructionNode*> instr);
        const std::string getIdentifier(void);
        std::vector<InstructionNode*> getInstructions(void);
        void print();
        void filePrint();
    private:
        std::string identifier;
        std::vector<InstructionNode*> instructions;
    protected:
};
class IRProgramNode{
    public:
        IRProgramNode(std::vector<IRFunctionNode*> fs);
        void print();
        void filePrint();
    private:
        std::vector<IRFunctionNode*> functions;
    protected:
};

class IRTree{
    private:
        AST ast_root;
        IRProgramNode* root;
        std::string traverseExpression(ExpressionNode* expression) ;
        std::vector<InstructionNode*> traverseStatement(StatementNode* statement);
        std::vector<IRFunctionNode*> traverseFunction(const std::vector<FunctionNode*> functions);
        IRProgramNode* traverseProgram(const ProgramNode* program);

    public:
        void transform();
        IRTree(AST a);
        void prettyPrint();
        void filePrint();
};

#endif