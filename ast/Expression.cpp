#include "Statement.h"
#include "Expression.h"

#include <numeric>
#include <utility>


    
    namespace monkey::ast {
        string Identifier::TokenLiteral() {
            return Token.getLiteral();
        }

        Identifier::Identifier(token::Token t, string v) {
            Token = std::move(t);
            Value = std::move(v);
        }

        string Identifier::String() {
            return Value;
        }

        string Identifier::getValue() {
            return Value;
        }

        IntegerLiteral::IntegerLiteral(token::Token token) {
            Token = std::move(token);
            value = 0;
        }

        string IntegerLiteral::TokenLiteral() {
            return Token.getLiteral();
        }

        string IntegerLiteral::String() {
            return Token.getLiteral();
        }

        void IntegerLiteral::setValue(long long int v) {
            this->value = v;
        }

        __attribute__((unused)) long long IntegerLiteral::getValue() const {
            return value;
        }

        string PrefixExpression::TokenLiteral() {
            return Token.getLiteral();
        }

        string PrefixExpression::String() {
            std::ostringstream oss;

            oss << "(";
            oss << Operator;
            oss << Right->String();
            oss << ")";

            return oss.str();
        }

        PrefixExpression::PrefixExpression(token::Token token, string op) {
            Token = std::move(token);
            Operator = std::move(op);
            Right = nullptr;
        }

        void PrefixExpression::setRightExpression(Expression *&right) {
            Right = right;
        }

        __attribute__((unused)) string PrefixExpression::getOperator() const {
            return Operator;
        }

        ast::Expression *PrefixExpression::getRightExpression() {
            return Right;
        }

        string InfixExpression::TokenLiteral() {
            return Token.getLiteral();
        }

        string InfixExpression::String() {
            std::ostringstream oss;

            oss << "(";
            oss << Left->String();
            oss << " " << Operator << " ";
            oss << Right->String();
            oss << ")";

            return oss.str();
        }

        void InfixExpression::setRightExpression(Expression* & right) {
            Right = right;
        }

        InfixExpression::InfixExpression(token::Token token, Expression* left, string op) {
            Token = std::move(token);
            Left = left;
            Operator = std::move(op);

            Right = nullptr;
        }

        __attribute__((unused)) Expression *InfixExpression::getLeft() {
            return Left;
        }

        __attribute__((unused)) Expression *InfixExpression::getRight() {
            return Right;
        }

        __attribute__((unused)) string InfixExpression::getOperator() {
            return Operator;
        }

        Boolean::Boolean(token::Token token, bool value) {
            Token = std::move(token);
            Value = value;
        }

        string Boolean::TokenLiteral() {
            return Token.getLiteral();
        }

        string Boolean::String() {
            return Token.getLiteral();
        }

        bool Boolean::getValue() const {
            return Value;
        }

        string IfExpression::TokenLiteral() {
            return Token.getLiteral();
        }

        string IfExpression::String() {
            std::ostringstream oss;

            oss << "if";
            oss << Condition->String();
            oss << " ";
            oss << Consequence->String();

            if (Alternative != nullptr) {
                oss << "else ";
                oss << Alternative->String();
            }

            return oss.str();
        }

        IfExpression::IfExpression(token::Token token) {
            Token = std::move(token);
            Consequence = nullptr;
            Condition = nullptr;
            Alternative = nullptr;
        }

        __attribute__((unused)) void IfExpression::setCondition(Expression *&condition) {
            Condition = condition;
        }

        __attribute__((unused)) void IfExpression::setConsequence(BlockStatement *&consequence) {
            Consequence = consequence;
        }

        __attribute__((unused)) void IfExpression::setAlternative(BlockStatement *&alternative) {
            Alternative = alternative;
        }

        __attribute__((unused)) Expression *& IfExpression::getCondition() { return Condition; }

        __attribute__((unused)) BlockStatement *& IfExpression::getConsequence() { return Consequence; }

        __attribute__((unused)) BlockStatement *& IfExpression::getAlternative() { return Alternative; }

        string FunctionLiteral::TokenLiteral() {
            return Token.getLiteral();
        }

        string FunctionLiteral::String() {
            std::ostringstream oss;

            std::vector<string> params;

            params.reserve(Parameters.size());

            for (const auto &p: Parameters) {
                params.push_back(p->String());
            }


            string paramsString = std::accumulate(
                    params.begin(),
                    params.end(),
                    std::string(),
                    [](const string& a, const string& b) -> string {
                        return a + (a.length() > 0 ? ", " : "") + b;
                    }
                    );

            oss << TokenLiteral();
            oss << "(";
            oss << paramsString;
            oss << ")";
            oss << Body->String();


            return oss.str();
        }

        FunctionLiteral::FunctionLiteral(token::Token token) {
            Token = std::move(token);
            Body = nullptr;
        }

        void FunctionLiteral::setParameters(std::vector<Identifier *> &parameters) {
            Parameters = parameters;
        }

        void FunctionLiteral::setBody(BlockStatement *&body) {
            Body = body;
        }

        std::vector<Identifier *> FunctionLiteral::getParameters() {
            return Parameters;
        }

        BlockStatement *FunctionLiteral::getBody() {
            return Body;
        }

        string CallExpression::TokenLiteral() {
            return Token.getLiteral();
        }

        string CallExpression::String() {
            std::ostringstream oss;

            std::vector<string> args;
            args.reserve(Arguments.size());

            for (const auto & argument : Arguments) {
                args.emplace_back(argument->String());
            }
            oss << Function->String();
            oss << "(";
            oss << std::accumulate(
                    args.begin(),
                    args.end(),
                    string(),
                    [] (const string& a, const string& b) -> string {
                        return a + (a.length() > 0 ? ", " : "") + b;
                    }
                    );
            oss << ")";

            return oss.str();
        }

        [[maybe_unused]] CallExpression::CallExpression(token::Token token) {
            Token = std::move(token);
            Function = nullptr;
        }

        [[maybe_unused]] Expression *CallExpression::getFunction() {
            return Function;
        }

        [[maybe_unused]] void CallExpression::setFunction(Expression *&function) {
            Function = function;
        }

        void CallExpression::setArguments(std::vector<Expression *> &arguments) {
            Arguments = arguments;
        }

        std::vector<Expression *> CallExpression::getArguments() {
            return Arguments;
        }

        CallExpression::CallExpression(token::Token token, Expression *&function) {
            Token = std::move(token);
            Function = function;
        }

        string StringLiteral::TokenLiteral() {
            return Token.getLiteral();
        }

        string StringLiteral::String() {
            return Token.getLiteral();
        }

        string StringLiteral::getValue() {
            return Value;
        }
    }

