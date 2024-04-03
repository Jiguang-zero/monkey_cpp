//
// Created by 86158 on 2024/3/26.
//

#ifndef MONKEY_TOKEN_H
#define MONKEY_TOKEN_H

#include "string"
#include <map>

using std::string;
using std::map;



    namespace monkey::token {
        typedef string TokenType;

        /**
         * �ʷ���Ԫ
         */
        class Token {
        protected:
            TokenType Type; // �ʷ���Ԫ������
            string Literal; // �ʷ���Ԫ��������


        public:
            /**
             * �հ׹��캯��������ʼ���������
             */
             Token() = default;

            /**
             * ���캯��
             * @param tokenType �ʷ���Ԫ������
             * @param literal �ʷ���Ԫ�������� string
             */
            Token(TokenType tokenType, string literal);

            /**
             * �������� TokenType
             * @param type
             */
            void setType(TokenType type);

            /**
             * ����������
             * @param literal string
             */
            void setLiteral(string literal);

            // ��ȡ���͵�ֵ
            TokenType getType();

            // ��ȡ��������ֵ
            string getLiteral();

            /**
             * �� token ת��Ϊ string
             * @return string
             */
            string String();
        };

        extern const TokenType ILLEGAL; // �Ƿ���Ԫ

        extern const TokenType TOKEN_EOF; // ������Ԫ

        // ��������
        extern const TokenType STRING; // �ַ�����Ԫ
        extern const TokenType INTEGER; // ������Ԫ
        //TODO: ���Ӹ���������

        // �������
        extern const TokenType ASSIGN; // ��ֵ���ŵ�Ԫ =
        extern const TokenType PLUS; // �Ӻ� +
        extern const TokenType MINUS; // ���� -
        extern const TokenType ASTERISK; // �Ǻ� *
        extern const TokenType SLASH; // б�� /(����)

        // �����������
        extern const TokenType BANG; // ��̾�� !
        extern const TokenType LT; // С�ں� < less than
        extern const TokenType GT; // ���ں� > greater than
        extern const TokenType EQ; // ��� == equal
        extern const TokenType NOT_EQ; // ����� != equal

        // �ָ���
        extern const TokenType COMMA; // ����
        extern const TokenType SEMICOLON; // �ֺ�

        // ��ʶ��
        extern const TokenType IDENT; // ��������

        extern const TokenType LPAREN; // ������ (
        extern const TokenType RPAREN; // ������ )
        extern const TokenType LBRACE; // ������� {
        extern const TokenType RBRACE; // �Ҵ�����

        // �ؼ���
        extern const TokenType FUNCTION; // �����ؼ��� fn
        extern const TokenType LET; // �ؼ��� let
        extern const TokenType TOKEN_TRUE; // �ؼ��� true
        extern const TokenType TOKEN_FALSE; // �ؼ��� false
        extern const TokenType IF; // �ؼ��� if
        extern const TokenType ELSE; // �ؼ��� else
        extern const TokenType RETURN; // �ؼ��� return

        extern map<string, TokenType > keywords;

        /**
         * �ӹؼ��� map �в��� ident �Ƿ��ǹؼ���
         * @param ident const string&
         * @return �������� TokenType
         */
        TokenType LookupIdent(const string& ident);

    } // token

// monkey

#endif //MONKEY_TOKEN_H
