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
/**
 * @brief TackyConstant : TackyVal
 * Represents constant values
 */
class TackyConstant : public TackyVal {
    private:
        /**
         * @brief String representing the Constant value
         * 
         */
        std::string value;

    public:
        /**
         * @brief Construct a new Tacky Constant object
         * 
         * @param val 
         */
        explicit TackyConstant(std::string val);
        /**
         * @brief Destroy the Tacky Constant object
         * 
         */
        ~TackyConstant() override;
        /**
         * @brief Get the Value object
         * 
         * @return std::string 
         */
        std::string getValue();
        /**
         * @brief Prints the TackyConstant in a specific format
         * 
         */
        void print() const override;
        /**
         * @brief Prints the TackyConstant in a specific format
         * 
         */
        void prettyPrint(int indent = 0) const override;
};

// ======================================================
//                     TackyVariable : TackyVal
// ======================================================
/**
 * @brief TackyVariable : TackyVal
 * Represents the temporary variable used when expanding expressions into
 * TAC format
 */
class TackyVariable : public TackyVal {
    private:
        /**
         * @brief A string representing the name of the register 
         * 
         */
        std::string variableIdentifier;

    public:
        /**
         * @brief Construct a new Tacky Variable object
         * 
         * @param varIdentifier 
         */
        explicit TackyVariable(std::string varIdentifier);
        /**
         * @brief Destroy the Tacky Variable object
         * 
         */
        ~TackyVariable() override;
        /**
         * @brief Get the Variable Identifier object
         * 
         * @return std::string 
         */
        std::string getVariableIdentifier();
        /**
         * @brief Prints the TackyVariable in a specific format
         * 
         */
        void print() const override;
        /**
         * @brief Prints the TackyVariable in a specific format
         * 
         */
        void prettyPrint(int indent = 0) const override;
};

// ======================================================
//                     Base: TackyInstruction
// ======================================================
/**
 * @brief Base: TackyInstruction
 * A base class representing instructions. All instructions inheirit from this class
 * 
 */
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
        /**
         * @brief TackyVal* val represents the expression the return statement returns 
         * 
         */
        TackyVal* val;

    public:
        /**
         * @brief Construct a new Tacky Return object
         * 
         * @param var 
         */
        explicit TackyReturn(TackyVal* var);
        /**
         * @brief Destroy the Tacky Return object
         * 
         */
        ~TackyReturn() override;

        /**
         * @brief Get the Var object
         * 
         * @return TackyVal* 
         */
        TackyVal* getVar();
        void print() const override;
        void prettyPrint(int indent = 0) const override;
};

// ======================================================
//                     TackyUnary : TackyInstruction
// ======================================================
/**
 * @brief TackyUnary : TackyInstruction
 * represents the Unary Operation. The AST UnaryNode get converted into a TackyUnary node
 * This conversion creates a dst(Usually of type TackyVariable* ). Turning the Unary operation 
 * in the AST into a TAC expression.
 * 
 */
class TackyUnary : public TackyInstruction {

    private:
        /**
         * @brief The Unary Operator being performed
         * 
         */
        UnaryOperator unary_operator;
        /**
         * @brief The value the Unary Operator is being applied to
         * 
         */
        TackyVal* src;
        /**
         * @brief The Destination of the Unary Operation
         * 
         */
        TackyVal* dst;

    public:
        TackyUnary(UnaryOperator unary_operator, TackyVal* src, TackyVal* dst);
        ~TackyUnary() override;
        /**
         * @brief Get the Unary Operator object
         * 
         * @return UnaryOperator 
         */
        UnaryOperator getUnaryOperator();
        /**
         * @brief Get the Src object
         * 
         * @return TackyVal* 
         */
        TackyVal* getSrc();
        /**
         * @brief Get the Dst object
         * 
         * @return TackyVal* 
         */
        TackyVal* getDst();

        void print() const override;
        void prettyPrint(int indent = 0) const override;
};

// ======================================================
//                     TackyFunction
// ======================================================
/**
 * @brief A node in the Tacky Tree representing a function in the source code
 * 
 */
class TackyFunction {
    private:
        /**
         * @brief std::string representing the name of the function
         * 
         */
        std::string identifier;
        /**
         * @brief The body of the function in TAC representation.
         * 
         */
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
/**
 * @brief A node in the Tacky representing the program.
 * Holds a reference to every function in the program.
 * 
 */
class TackyProgram {
private:
    std::vector<TackyFunction*> functions;

public:
    explicit TackyProgram(std::vector<TackyFunction*> functions);
    ~TackyProgram();
    /**
     * @brief Get the Functions object
     * 
     * @return std::vector<TackyFunction*> 
     */
    std::vector<TackyFunction*> getFunctions();
    void print() const;
    void prettyPrint(int indent = 0) const;
};

// ======================================================
//                     TackyGenerator
// ======================================================
/**
 * @brief The class that generates the Tacky Tree
 * 
 */
class TackyGenerator {
    private:
        int temp_counter;

    public:
        TackyGenerator();
        std::string make_temporary();

        TackyVal* convertExpression(ExpressionNode* expression, std::vector<TackyInstruction*>& instructions);
        std::vector<TackyInstruction*> convertStatement(StatementNode* statement);
        TackyFunction* convertFunction(FunctionNode* function);
        /**
         * @brief Takes in the root of an AST and produces the root of a Tacky tree
         * 
         * @param ast 
         * @return TackyProgram* 
         */
        TackyProgram* convertProgram(AST* ast);
};

#endif // TACKY_HPP
