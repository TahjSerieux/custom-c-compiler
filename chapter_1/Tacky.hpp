#ifndef TACKY_H
#define TACKY_H
#include "AST.hpp"

class TackyVal{
    public:
        virtual ~TackyVal()=0;
    private:
};

class TackyConstant: public TackyVal{
    public:
        std::string getValue();
        TackyConstant(std::string val);
        ~TackyConstant();
    private:
        std::string value;
};

class TackyVariable:public TackyVal{
    public:
        std::string getVariableIdentifier();
        TackyVariable();
        ~TackyVariable();
    private:
        std::string variableIdentifier();
};


class TackyInstruction{
    public:
        virtual ~TackyInstruction() = 0;
    private:
};

class TackyReturn: public TackyInstruction{
    public:

        TackyReturn();
        ~TackyReturn();
        TackyVal* getExpression();
    private:
        TackyVal* val;
};

class TackyUnary: public TackyVal{
    public:
        TackyUnary();
        ~TackyUnary();
        UnaryOperator getUnaryOperator();
        TackyVariable getSrc();
        TackyVariable getDst();
    private:
        UnaryOperator unary_operator;
        TackyVariable src;
        TackyVariable dst;

};

class TackyFunction{
    public:
        TackyFunction();
        std::string getIdentifier();
        std::vector<TackyInstruction*> getBody();
    private:
        std::string identifier;
        std::vector<TackyInstruction*> body;
};
// ======================================================
//                     TackyProgram
// ======================================================
class TackyProgram{
    public:
        TackyProgram(std::vector<TackyFunction*> functions);
        std::vector<TackyFunction*> getFunctions();
    private:
        std::vector<TackyFunction*> functions;
};
// ======================================================
//                     TackyGenerator
// ======================================================
class TackyGenerator{
    public:
        TackyGenerator();
        std::string make_temporary();
        void convertExpression(ExpressionNode* expression,std::vector<TackyInstruction*>& instructions);
        std::vector<TackyInstruction*> convertInstructions(StatementNode* statement);
        TackyFunction* convertFunction(FunctionNode*  function);
        TackyProgram* convertProgram(AST* ast);
    private:
        int temp_counter;
};

#endif