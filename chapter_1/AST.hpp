#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <iostream>
#include "Token.hpp"

// ======================================================
//                     Enums
// ======================================================

enum ExpressionType { CONSTANT };
enum StatementType  { RETURN };


// ======================================================
//                     ExpressionNode
// ======================================================
// Abstract Base class for all expressions (statements that evaluate to a value)
class ExpressionNode {
public:
    virtual void print() = 0;
    ExpressionType getType() const;

protected:
    ExpressionNode(ExpressionType t);
    virtual const std::string getValue() = 0;

    ExpressionType type;
};


// ======================================================
//                     ConstantNode
// ======================================================
class ConstantNode : public ExpressionNode {
private:
    std::string value;

public:
    ConstantNode(std::string value);
    ~ConstantNode();

    void print() override;
    const std::string getValue() override;
};


// ======================================================
//                     StatementNode
// ======================================================
// Abstract Base class for all statements
// (self-contained units of execution, i.e., valid lines of code)
class StatementNode {
public:
    virtual ~StatementNode() = 0;
    virtual void print() = 0;

    StatementType getType();
    virtual const ExpressionNode* getExpression() const = 0;

protected:
    StatementNode(StatementType t);
    const StatementType type;
};


// ======================================================
//                     ReturnNode
// ======================================================
class ReturnNode : public StatementNode {
private:
    ExpressionNode* exp;

public:
    ReturnNode(ExpressionNode* exp);

    void print() override;
    ExpressionNode* getExpression() const override;
};


// ======================================================
//                     FunctionNode
// ======================================================
class FunctionNode {
private:
    std::string identifier;
    StatementNode* statement;

public:
    FunctionNode(std::string identifier, StatementNode* statement);

    void print();
    std::string getIdentifer();
    StatementNode* getStatement();
};


// ======================================================
//                     ProgramNode
// ======================================================
class ProgramNode {
private:
    std::vector<FunctionNode*> functions;

public:
    ProgramNode(std::vector<FunctionNode*> funcs);

    void print() const;
    std::vector<FunctionNode*> getFunction() const;
};


// ======================================================
//                     AST
// ======================================================
class AST {
private:
    const ProgramNode* const root;
    std::vector<Token> Tokens;

public:
    AST(ProgramNode* root);

    const ProgramNode* getRoot() const;
    void PrettyPrint() const;
};

#endif
