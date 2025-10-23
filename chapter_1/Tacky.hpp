#ifndef TACKY_HPP
#define TACKY_HPP

#include "AST.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

// // Helper forward (not strictly necessary but clarifies intent)
// static inline void printIndent(int indent) {
//     std::cout << std::string(indent * 2, ' ');
// }

// ======================================================
//                     Base: TackyVal
// ======================================================
class TackyVal {
public:
    virtual ~TackyVal() = 0;
    virtual void print() const = 0;
    virtual void prettyPrint(int indent = 0) const = 0;
};
inline TackyVal::~TackyVal() {}

// ======================================================
//                     TackyConstant : TackyVal
// ======================================================
class TackyConstant : public TackyVal {
private:
    std::string value;

public:
    explicit TackyConstant(std::string val);
    ~TackyConstant() override;

    std::string getValue();
    void print() const override;
    void prettyPrint(int indent = 0) const override;
};

// ======================================================
//                     TackyVariable : TackyVal
// ======================================================
class TackyVariable : public TackyVal {
private:
    std::string variableIdentifier;

public:
    explicit TackyVariable(std::string varIdentifier);
    ~TackyVariable() override;

    std::string getVariableIdentifier();
    void print() const override;
    void prettyPrint(int indent = 0) const override;
};

// ======================================================
//                     Base: TackyInstruction
// ======================================================
class TackyInstruction {
    public:
        virtual ~TackyInstruction() = 0;
        virtual void print() const = 0;
        virtual void prettyPrint(int indent = 0) const = 0;
};
inline TackyInstruction::~TackyInstruction() {}

// ======================================================
//                     TackyReturn : TackyInstruction
// ======================================================
class TackyReturn : public TackyInstruction {
    private:
        TackyVal* val;

    public:
        explicit TackyReturn(TackyVal* var);
        ~TackyReturn() override;

        TackyVal* getVar();
        void print() const override;
        void prettyPrint(int indent = 0) const override;
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
    ~TackyUnary() override;

    UnaryOperator getUnaryOperator();
    TackyVal* getSrc();
    TackyVal* getDst();

    void print() const override;
    void prettyPrint(int indent = 0) const override;
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
    ~TackyFunction();

    std::string getIdentifier();
    std::vector<TackyInstruction*> getBody();

    void print() const;
    void prettyPrint(int indent = 0) const;
};

// ======================================================
//                     TackyProgram
// ======================================================
class TackyProgram {
private:
    std::vector<TackyFunction*> functions;

public:
    explicit TackyProgram(std::vector<TackyFunction*> functions);
    ~TackyProgram();

    std::vector<TackyFunction*> getFunctions();
    void print() const;
    void prettyPrint(int indent = 0) const;
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
    std::vector<TackyInstruction*> convertStatement(StatementNode* statement);
    TackyFunction* convertFunction(FunctionNode* function);
    TackyProgram* convertProgram(AST* ast);
};

#endif // TACKY_HPP
