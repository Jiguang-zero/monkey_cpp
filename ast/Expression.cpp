#include "Expression.h"

#include <utility>

namespace monkey {
    
    namespace ast {
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
    }

}