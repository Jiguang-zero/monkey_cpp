#include "Parser.h"
#include <iostream>
#include <cstdlib>


    namespace monkey::parser {
        Parser::Parser(lexer::Lexer* _l) {
            l = _l;
        }

            Parser* Parser::New(lexer::Lexer* l) {
            auto* p = new Parser(l);

            // 注册前缀函数
            p->registerPrefix(token::IDENT, &Parser::parseIdentifier);
            p->registerPrefix(token::INTEGER, &Parser::parseIntegerLiteral);
            p->registerPrefix(token::BANG, &Parser::parsePrefixExpression);
            p->registerPrefix(token::MINUS, &Parser::parsePrefixExpression);
            p->registerPrefix(token::TOKEN_TRUE, &Parser::parseBoolean);
            p->registerPrefix(token::TOKEN_FALSE, &Parser::parseBoolean);
            p->registerPrefix(token::LPAREN, &Parser::parseGroupedExpression);
            p->registerPrefix(token::IF, &Parser::parseIfExpression);
            p->registerPrefix(token::FUNCTION, &Parser::parseFunctionLiteral);
            p->registerPrefix(token::STRING, &Parser::parseStringLiteral);

            // 注册中缀函数
            p->registerInfix(token::PLUS, &Parser::parseInfixExpression);
            p->registerInfix(token::MINUS, &Parser::parseInfixExpression);
            p->registerInfix(token::SLASH, &Parser::parseInfixExpression);
            p->registerInfix(token::ASTERISK, &Parser::parseInfixExpression);
            p->registerInfix(token::EQ, &Parser::parseInfixExpression);
            p->registerInfix(token::NOT_EQ, &Parser::parseInfixExpression);
            p->registerInfix(token::LT, &Parser::parseInfixExpression);
            p->registerInfix(token::GT, &Parser::parseInfixExpression);
            p->registerInfix(token::LPAREN, &Parser::parseCallExpression);


            // 设置 curToken 与 peekToken
            p->nextToken();
            p->nextToken();

            return p;
        }

        void Parser::nextToken() {
            curToken = peekToken;
            peekToken = l->NextToken();
        }

        bool Parser::curTokenIs(const token::TokenType &t) {
            return curToken.getType() == t;
        }

        bool Parser::peekTokenIs(const token::TokenType &t) {
            return peekToken.getType() == t;
        }

        bool Parser::expectPeek(const token::TokenType &t) {
            if (peekTokenIs(t)) {
                nextToken();
                return true;
            }
            else {
                peekError(t);
                //TODO 错误输出测试
                return false;
            }
        }

        __attribute__((unused)) vector<string> Parser::getErrors() {
            return errors;
        }

        void Parser::peekError(const token::TokenType &t) {
            string msg = "expected next token to be " + t + ", got "
                    + peekToken.getType() + " instead";
            errors.emplace_back(msg);
        }

        void Parser::noPrefixParseFnError(const token::TokenType& t) {
            auto msg = "no prefix parse function for " + t + " found.";
            errors.emplace_back(msg);
        }

        ast::Program *Parser::ParseProgram() {
            auto * program = new ast::Program();

            (program)->setStatements(vector<ast::Statement*>());

            while (curToken.getType() != token::TOKEN_EOF) {
                auto *stmt = new ast::Statement();
                stmt = parseStatement();

                if (stmt != nullptr) {
                    (*program).getStatements().emplace_back(stmt);

                }
                nextToken();
            }

            return program;
        }

        ast::Statement *Parser::parseStatement() {
            token::TokenType type = curToken.getType();

            if (type == token::LET) {
                return parseLetStatement();
            }
            else if (type == token::RETURN) {
                return parseReturnStatement();
            }

            else {
                return parseExpressionStatement();
            }
        }

        ast::LetStatement *Parser::parseLetStatement() {
            auto* stmt = new ast::LetStatement(curToken);

            if (!expectPeek(token::IDENT)) {
                return nullptr;
            }

            (*stmt).setName(new ast::Identifier(curToken, curToken.getLiteral()));

            if (!expectPeek(token::ASSIGN)) {
                return nullptr;
            }


            nextToken();

            auto * value = new ast::Expression();
            value = parseExpression(LOWEST);

            stmt->setValue(value);

            if (peekTokenIs(token::SEMICOLON)) {
                nextToken();
            }


            return stmt;
        }

        ast::ReturnStatement *Parser::parseReturnStatement() {
            auto* stmt = new ast::ReturnStatement(curToken);

            nextToken();

            auto * value = new ast::Expression();
            value = parseExpression(LOWEST);
            stmt->setReturnValue(value);

            if (peekTokenIs(token::SEMICOLON)) {
                nextToken();
            }


            return stmt;
        }

        ast::ExpressionStatement *Parser::parseExpressionStatement() {
            auto* stmt = new ast::ExpressionStatement(curToken);

            auto expression = new ast::Expression();
            expression = parseExpression(PRECEDENCE::LOWEST);

            (*stmt).setExpression(expression);


            if (peekTokenIs(token::SEMICOLON)) {
                nextToken();
            }
            return stmt;
        }

        ast::Expression *Parser::parseExpression(const PRECEDENCE &precedence) {
            auto prefix = prefixParseFns[curToken.getType()];
            if (prefix == nullptr) {
                noPrefixParseFnError(curToken.getType());
                return nullptr;
            }

            auto* leftExp = (this->*prefix)();

            while (!peekTokenIs(token::SEMICOLON) && precedence < peekPrecedence()) {
                auto infix = infixParseFns[peekToken.getType()];
                if (infix == nullptr) {
                    return leftExp;
                }

                nextToken();

                leftExp = (this->*infix)(leftExp);
            }

            return leftExp;
        }

        ast::Expression* Parser::parseIdentifier() {
            return new ast::Identifier(curToken, curToken.getLiteral());
        }

        ast::Expression *Parser::parseIntegerLiteral() {
            auto * integerLiteral = new ast::IntegerLiteral(curToken);

            char* endPtr;
            long long value = strtoll(curToken.getLiteral().c_str(), &endPtr, 10);
            if (endPtr == curToken.getLiteral().c_str()) {
                errors.emplace_back("could not parse " + curToken.getLiteral() + " as a number(64)");
            }

            integerLiteral->setValue(value);

            return integerLiteral;
        }


        ast::Expression *Parser::parsePrefixExpression() {
            auto * prefixExpression = new ast::PrefixExpression(curToken, curToken.getLiteral());

            nextToken();

            auto * right = new ast::Expression();
            right = parseExpression(PRECEDENCE::PREFIX);
            prefixExpression->setRightExpression(right);

            return prefixExpression;
        }

        PRECEDENCE Parser::peekPrecedence() {
            return getPrecedenceWithTokenType(peekToken.getType());
        }

        PRECEDENCE Parser::curPrecedence() {
            return getPrecedenceWithTokenType(curToken.getType());
        }

        ast::Expression * Parser::parseInfixExpression(ast::Expression* left) {
            auto * infixExpression = new ast::InfixExpression(
                    curToken,
                    left,
                    curToken.getLiteral()
                );

            auto precedence = curPrecedence();
            nextToken();

            auto * right = new ast::Expression();
            right = parseExpression(precedence);

            infixExpression->setRightExpression(right);

            return infixExpression;
        }


        PRECEDENCE Parser::getPrecedenceWithTokenType(const token::TokenType& type) {
            if (type == token::EQ || type == token::NOT_EQ) {
                return EQUALS;
            }
            if (type == token::LT || type == token::GT) {
                return LESS_GREATER;
            }
            if (type == token::PLUS || type == token::MINUS) {
                return SUM;
            }
            if (type == token::SLASH || type == token::ASTERISK) {
                return PRODUCT;
            }
            if (type == token::LPAREN) {
                return CALL;
            }
            return LOWEST;
        }

        ast::Expression *Parser::parseBoolean() {
            return new ast::Boolean(curToken, curTokenIs(token::TOKEN_TRUE));
        }

        ast::Expression *Parser::parseGroupedExpression() {
            // 遇到括号选择下一个词法单元
            nextToken();

            auto * expression = parseExpression(LOWEST);

            if (!expectPeek(token::RPAREN)) {
                return nullptr;
            }


            return expression;
        }

        ast::Expression *Parser::parseIfExpression() {
            auto * expression = new ast::Expression();

            auto * ifExpression = new ast::IfExpression(curToken);

            if (!expectPeek(token::LPAREN)) {
                delete ifExpression;
                delete expression;

                return nullptr;
            }

            nextToken();
            auto condition = new ast::Expression();
            condition = parseExpression(LOWEST);
            ifExpression->setCondition(condition);


            if (!expectPeek(token::RPAREN)) {
                delete condition;
                condition = nullptr;
                delete ifExpression;
                delete expression;
                return nullptr;
            }

            if (!expectPeek(token::LBRACE)) {
                delete condition;
                condition = nullptr;
                delete ifExpression;
                delete expression;
                return nullptr;
            }

            auto * consequence = new ast::BlockStatement();
            consequence = parseBlockStatement();
            ifExpression->setConsequence(consequence);

            if (peekTokenIs(token::ELSE)) {
                nextToken();

                if (!expectPeek(token::LBRACE)) {
                    delete consequence;
                    consequence = nullptr;
                    delete condition;
                    condition = nullptr;
                    delete ifExpression;
                    delete expression;
                    return nullptr;
                }

                auto * alternative = new ast::BlockStatement();
                alternative = parseBlockStatement();
                ifExpression->setAlternative(alternative);
            }


            expression = ifExpression;

            return expression;
        }

        ast::BlockStatement *Parser::parseBlockStatement() {
            auto block = new ast::BlockStatement(curToken);
            block->setStatements(vector<ast::Statement*>());

            // 获取 { 下一个词法单元
            nextToken();

            while (!curTokenIs(token::RBRACE) && !curTokenIs(token::TOKEN_EOF)) {
                auto * stmt = new ast::Statement;
                stmt = parseStatement();
                if (stmt != nullptr) {
                    block->getStatements().emplace_back(stmt);
                }
                nextToken();
            }

            return block;
        }

        ast::Expression *Parser::parseFunctionLiteral() {
            auto * expression = new ast::Expression();

            auto * functionLiteral = new ast::FunctionLiteral(curToken);

            if (!expectPeek(token::LPAREN)) {
                delete functionLiteral;
                delete expression;
                return nullptr;
            }

            auto parameters = parseFunctionParameters();
            functionLiteral->setParameters(parameters);

            if (!expectPeek(token::LBRACE)) {
                // 释放局部变量 parameters 存放的指针
                for (const auto & i : parameters) {
                    delete i;
                }

                delete functionLiteral;
                delete expression;

                return nullptr;
            }

            auto * body = new ast::BlockStatement();
            body = parseBlockStatement();
            functionLiteral->setBody(body);

            expression = functionLiteral;

            return expression;
        }

        vector<ast::Identifier *> Parser::parseFunctionParameters() {
            vector<ast::Identifier*> identifiers;

            if (peekTokenIs(token::RPAREN)) {
                // 跳到右括号词法单元
                nextToken();
                return identifiers;
            }

            // 跳过有括号词法单元
            nextToken();

            identifiers.emplace_back(new ast::Identifier(curToken, curToken.getLiteral()));

            while (peekTokenIs(token::COMMA)) {
                // 调到下一个参数
                nextToken();
                nextToken();

                //TODO: 处理参数个数与逗号个数不匹配的问题

                identifiers.emplace_back(new ast::Identifier(curToken, curToken.getLiteral()));
            }

            if (!expectPeek(token::RPAREN)) {
                // 错误， 返回0个参数
                //TODO: 错误处理

                for (const auto & i : identifiers) {
                    delete i;
                }

                return {};
            }

            return identifiers;
        }

        ast::Expression* Parser::parseCallExpression(ast::Expression *function) {
            auto * callExpression = new ast::CallExpression(curToken, function);

            vector<ast::Expression*> arguments;
            arguments = parseCallArguments();
            callExpression->setArguments(arguments);

            return callExpression;
        }

        vector<ast::Expression*> Parser::parseCallArguments() {
            vector<ast::Expression*> args;

            if (peekTokenIs(token::RPAREN)) {
                nextToken();
                return args;
            }

            nextToken();
            auto * firstArgument = new ast::Expression();
            firstArgument = parseExpression(LOWEST);
            args.emplace_back(firstArgument);

            while (peekTokenIs(token::COMMA)) {
                nextToken();
                nextToken();

                //TODO: 错误处理 add(a, b, ) // 这种情况

                auto * argument = new ast::Expression();
                argument = parseExpression(LOWEST);
                args.emplace_back(argument);
            }

            if (!expectPeek(token::RPAREN)) {
                // 释放空间
                for (const auto & a : args) {
                    delete a;
                }
                delete firstArgument;
                firstArgument = nullptr;

                return {};
            }

            return args;
        }

        ast::Expression *Parser::parseStringLiteral() {
            return new ast::StringLiteral(curToken, curToken.getLiteral());
        }
    }

