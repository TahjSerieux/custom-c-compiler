#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <iostream>
#include "Token.hpp"

// ======================================================
//                     Enums
// ======================================================

enum class ExpressionType { CONSTANT,UNARY,BINARY };
enum class StatementType  { RETURN };
enum class UnaryOperator{Complement, Negation,Error};
enum class BinaryOperator{Add,Subtract,Multiply,Divide,Remainder,Error};
std::string unary_operator_to_string(UnaryOperator op);
// ======================================================
//                     ExpressionNode
// ======================================================
class ExpressionNode {
    public:
        virtual void print() = 0;
        ExpressionType getType() const;

        virtual const std::string getValue() = 0;
    protected:
        ExpressionNode(ExpressionType t);

        ExpressionType type;
};


// ======================================================
//                     ConstantNode:ExpressionNode
// ======================================================
class ConstantNode : public ExpressionNode {
    private:
        /**
         * @brief The value of the constant
         * 
         */
        std::string value;

    public:
        /**
         * @brief Construct a new Constant Node object
         * 
         * @param value 
         */
        ConstantNode(std::string value);
        /**
         * @brief Destroy the Constant Node object
         * 
         */
        ~ConstantNode();
        /**
         * @brief Prints the value of the ConstantNode in a specific format
         * 
         */
        void print() override;
        /**
         * @brief Get the Value object
         * 
         * @return const std::string 
         */
        const std::string getValue() override;
    };
// ======================================================
//                     UnaryNode:ExpressionNode
// ======================================================
    
class UnaryNode: public ExpressionNode{
    private:
        /**
         * @brief An enum representing the type of unary operator
         * 
         */
        UnaryOperator unary_operator;
        /**
         * @brief Tjhe expression the unary operator will be applied to.
         * 
         */
        ExpressionNode* exp;
    public:
        /**
         * @brief Construct a new Unary Node object
         * 
         * @param exp 
         */
        UnaryNode(UnaryOperator unary_operator, ExpressionNode* exp);
        /**
         * @brief Destroy the Unary Node object
         * 
         */
        ~UnaryNode();
        /**
         * @brief get the unary_operator
         * @return unary_operator
         */
        UnaryOperator get_unary_operator();
        /**
         * @brief Get the Expression object
         * 
         * @return ExpressionNode* 
         */
        ExpressionNode* getExpression();
        /**
         * @brief Prints the value of the UnaryNode
         * 
         */
        void print() override;
        /**
         * @brief Get the Value object
         * 
         * @return const std::string 
         */
        const std::string getValue() override;
};
// ======================================================
//                     BinaryNode:ExpressionNode
// ======================================================
class BinaryNode: public ExpressionNode
{
    private:
        /**
         * @brief An enum representing the type of binary operator
         * 
         */
        BinaryOperator binary_operator;
        /**
         * @brief The
         *  expression the unary operator will be applied to.
         * 
         */
        ExpressionNode* firstExpression;
        ExpressionNode* secondExpression;

    public:
        BinaryNode(BinaryOperator binary_operator,ExpressionNode* firstExpression,ExpressionNode* secondExpression);
        ~BinaryNode();
        ExpressionNode* getFirstExpression();
        ExpressionNode* getSecondExpression();
        BinaryOperator getBinaryOperator();
         void print() override;
        /**
         * @brief Get the Value object
         * 
         * @return const std::string 
         */
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

        /**
         * @brief Get the Type object. All derived classes inherits this function.
         * 
         * @return StatementType 
         */
        StatementType getType();
        virtual const ExpressionNode* getExpression() const = 0;

    protected:
        StatementNode(StatementType t);
        /**
         * @brief An enum that represents the type of Statement the StatementNode is.
         * 
         */
        const StatementType type;
};


// ======================================================
//                     ReturnNode:StatementNode
// ======================================================
class ReturnNode : public StatementNode {
    private:
    /**
     * @brief Expression for the Return node. The value it returns.
     * An expression is something that evaluatees to a single value
     * 
     */
        ExpressionNode* exp;

    public:
        /**
         * @brief Construct a new Return Node object
         * 
         * @param exp 
         */
        ReturnNode(ExpressionNode* exp);

        void print() override;
        /**
         * @brief Get the Expression object
         * 
         * @return ExpressionNode* 
         */
        ExpressionNode* getExpression() const override;
};


// ======================================================
//                     FunctionNode
// ======================================================
class FunctionNode {
private:
    /**
     * @brief Identifier for the function
     * 
     */
    std::string identifier;
    /**
     * @brief The statement of the function. So far only supports one statement
     * A statement is an operation that does something.
     */
    StatementNode* statement;

public:
    /**
     * @brief Construct a new Function Node object
     * 
     * @param identifier 
     * @param statement 
     */
    FunctionNode(std::string identifier, StatementNode* statement);

    void print();
    /**
     * @brief Get the Identifer object
     * 
     * @return std::string 
     */
    std::string getIdentifer();
    /**
     * @brief Get the Statement object
     * 
     * @return StatementNode* 
     */
    StatementNode* getStatement();
};


// ======================================================
//                     ProgramNode
// ======================================================
class ProgramNode {
private:
    /**
     * @brief Vector of type std::vector<FunctionNode*>. Each pointer in the vector
     * represents a function in the program.
     * 
     */
    std::vector<FunctionNode*> functions;

public:
    /**
     * @brief Construct a new Program Node object
     * 
     * @param funcs 
     */
    ProgramNode(std::vector<FunctionNode*> funcs);
    /**
     * @brief Goes through each Function Node in std::vector<FunctionNode*> functions
     * each FunctionNode in functions represents a function in the program.
     * 
     */
    void print() const;
    /**
     * @brief Get the Functions object
     * 
     * @return std::vector<FunctionNode*> 
     */
    std::vector<FunctionNode*> getFunctions() const;
};


// ======================================================
//                     AST
// ======================================================
class AST {
private:
    const ProgramNode* const root;
    std::vector<Token> Tokens;

public:
    /**
     * @brief Construct a new AST object
     * 
     * @param root 
     */
    AST(ProgramNode* root);
    /**
     * @brief Construct a new AST object
     * 
     */
    AST();
    /**
     * @brief Get the Root object
     * 
     * @return const ProgramNode* 
     */
    const ProgramNode* getRoot() const;
    /**
     * @brief Public function for printing the Abstract Syntax Tree
     * 
     */
    void PrettyPrint() const;
};

#endif
