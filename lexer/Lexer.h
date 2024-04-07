//
// Created by 86158 on 2024/3/26.
//

#ifndef MONKEY_LEXER_H
#define MONKEY_LEXER_H

#include "string"
#include "../token/Token.h"

using std::string;




        namespace monkey::lexer {

        /**
         * �ʷ�������
         */
        class Lexer {
        protected:
            string input; // �ʷ�������������
            int position; // �ַ����ĵ�ǰλ��
            int readPosition; // �ַ����ĵ�ǰ��ȡλ��(��ǰ�ַ�����һ���ַ�)
            char ch; // ��ǰ�ַ�


        private:
            /**
             * �ж�һ���ַ��Ƿ�����ĸ(����˵�Ǳ�ʶ����һ����)
             * @param ch
             * @return bool
             * TODO: ���������»��ߵ�λ��
             */
            static bool isLetter(char ch);

            /**
             * �ж��ַ��Ƿ�������
             * @param ch
             * @return
             */
            static bool isDigit(char ch);

            /**
             * ��ȡ��ʶ��
             * @return ���ر�ʶ���ַ�����������
             */
            string readIdentifier();

            /**
             * �����հ��ַ� ' ', '\t', '\n', '\r'
             */
            void skipWhiteSpace();

            /**
             * ��ȡ����
             * @return �������ֵ� string
             */
            string readNumber();

            /**
             * ��ȡ�ַ�
             * ֻ�����ڶ�ȡ��Unicode �ַ�
             * TODO: ����Unicode�ַ��Ķ�ȡ
             */
            void readChar();

            /**
             * ������һ���ַ������ƶ�λ�� position
             * @return ������һ���ַ�
             */
            char peekChar();

            /**
             * ��ȡ�ַ���
             * @return string
             */
            string readString();

        public:
            /**
             * ���캯���������ַ���
             * @param _input
             */
            explicit Lexer(string _input);



            /**
             * ��ȡ��һ���ʷ���Ԫ
             * @return ���شʷ���Ԫ
             */
            token::Token NextToken();

            /**
             * �½�һ���ʷ�������
             * @param input string, �ַ���
             * @return Lexer*, ָ��ʷ���������ָ��
             */
            static Lexer* New(string input);

            /**
             * �½�һ���ʷ���Ԫ
             * @param tokenType �ʷ���Ԫ����
             * @param ch �ʷ���Ԫ��������
             * @return Token ����
             */
            static token::Token newToken(token::TokenType tokenType, char ch);

        };


    } // lexer
// monkey

#endif //MONKEY_LEXER_H
