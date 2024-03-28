//
// Created by 86158 on 2024/3/26.
//

#ifndef MONKEY_TOKEN_H
#define MONKEY_TOKEN_H

#include "string"
#include <map>

using std::string;
using std::map;


namespace monkey {


    namespace token {
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

        extern TokenType ILLEGAL; // �Ƿ���Ԫ

        extern TokenType TOKEN_EOF; // ������Ԫ

        // ��������
        extern TokenType STRING; // �ַ�����Ԫ
        extern TokenType INTEGER; // ������Ԫ
        //TODO: ���Ӹ���������

        // �������
        extern TokenType ASSIGN; // ��ֵ���ŵ�Ԫ =
        extern TokenType PLUS; // �Ӻ� +
        extern TokenType MINUS; // ���� -
        extern TokenType ASTERISK; // �Ǻ� *
        extern TokenType SLASH; // б�� /(����)

        // �����������
        extern TokenType BANG; // ��̾�� !
        extern TokenType LT; // С�ں� < less than
        extern TokenType GT; // ���ں� > greater than
        extern TokenType EQ; // ��� == equal
        extern TokenType NOT_EQ; // ����� != equal

        // �ָ���
        extern TokenType COMMA; // ����
        extern TokenType SEMICOLON; // �ֺ�

        // ��ʶ��
        extern TokenType IDENT; // ��������

        extern TokenType LPAREN; // ������ (
        extern TokenType RPAREN; // ������ )
        extern TokenType LBRACE; // ������� {
        extern TokenType RBRACE; // �Ҵ�����

        // �ؼ���
        extern TokenType FUNCTION; // �����ؼ��� fn
        extern TokenType LET; // �ؼ��� let
        extern TokenType TOKEN_TRUE; // �ؼ��� true
        extern TokenType TOKEN_FALSE; // �ؼ��� false
        extern TokenType IF; // �ؼ��� if
        extern TokenType ELSE; // �ؼ��� else
        extern TokenType RETURN; // �ؼ��� return

        extern map<string, TokenType > keywords;

        /**
         * �ӹؼ��� map �в��� ident �Ƿ��ǹؼ���
         * @param ident const string&
         * @return �������� TokenType
         */
        TokenType LookupIdent(const string& ident);

    } // token

} // monkey

#endif //MONKEY_TOKEN_H
