#ifndef TACKY_H
#define TACKY_H
#include "AST.hpp"
#include <iostream>

// ======================================================
//                     Base: TackyVal
// ======================================================
class TackyVal {
public:
    virtual ~TackyVal() = 0;
    virtual void print() const = 0;
};
inline TackyVal::~TackyVal() {}

// ======================================================
//                     TackyConstant : TackyVal
// ======================================================
class TackyConstant : public TackyVal {
    private:
        std::string value;
    public:
        TackyConstant(std::string val);
        ~TackyConstant();

        std::string getValue();
        void print() const override;
};

// ======================================================
//                     TackyVariable : TackyVal
// ======================================================
class TackyVariable : public TackyVal {
private:
    std::string variableIdentifier;
public:
    TackyVariable(std::string varIdentifier);
    ~TackyVariable();

    std::string getVariableIdentifier();
    void print() const override;
};

// ======================================================
//                     Base: TackyInstruction
// ======================================================

class TackyInstruction {
    public:
        virtual ~TackyInstruction() = 0;
        virtual void print() const = 0;
};
inline TackyInstruction::~TackyInstruction() {}

// ======================================================
//                     TackyReturn : TackyInstruction
// ======================================================
class TackyReturn : public TackyInstruction {
private:
    TackyVal* var;
public:
    TackyReturn(TackyVal* var);
    ~TackyReturn();

    TackyVal* getVar();
    void print() const override;
};

// ======================================================
//                     TackyUnary : TackyInstruction
// ======================================================
class TackyUnary : public TackyInstruction {
private:
    UnaryOperator unary_operator;
    TackyVal* src;
    TackyVal* dst;
public:
    TackyUnary(UnaryOperator unary_operator, TackyVal* src, TackyVal* dst);
    ~TackyUnary();

    UnaryOperator getUnaryOperator();
    TackyVal* getSrc();
    TackyVal* getDst();

    void print() const override;
};

// ======================================================
//                     TackyFunction
// ======================================================
class TackyFunction {
private:
    std::string identifier;
    std::vector<TackyInstruction*> body;
public:
    TackyFunction(std::string identifier, std::vector<TackyInstruction*> body);
    std::string getIdentifier();
    std::vector<TackyInstruction*> getBody();

    void print() const;
};

// ======================================================
//                     TackyProgram
// ======================================================
class TackyProgram {
private:
    std::vector<TackyFunction*> functions;
public:
    TackyProgram(std::vector<TackyFunction*> functions);
    std::vector<TackyFunction*> getFunctions();

    void print() const;
};

// ======================================================
//                     TackyGenerator
// ======================================================
class TackyGenerator {
private:
    int temp_counter;
public:
    TackyGenerator();
    std::string make_temporary();

    TackyVal* convertExpression(ExpressionNode* expression, std::vector<TackyInstruction*>& instructions);
    std::vector<TackyInstruction*> convertInstructions(StatementNode* statement);
    TackyFunction* convertFunction(FunctionNode*  function);
    TackyProgram* convertProgram(AST* ast);
};

#endif
