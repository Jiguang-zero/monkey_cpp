
#ifndef MONKEY_PROGRAM_H
#define MONKEY_PROGRAM_H

#include "ast.h"
#include "Statement.h"

#include <vector>

using std::vector;

namespace monkey
{
    namespace ast
    {
        class Program : virtual public Node{
            protected:
                vector<Statement*> Statements;

            public:
                // 默认构造函数
                Program() = default;

                /**
                 * 构造函数，传递 statements
                 * @param statements 要传入临时新建的量，所以不传入引用
                 */
                __attribute__((unused)) explicit Program(vector<Statement*> statements);

                // 从外部设置 Statements
                void setStatements(vector<Statement*> statements);

                // 返回 Statements 以供外部修改
                vector<Statement*>& getStatements();

                /**
                 * Program 的 TokenLiteral() 定义
                 */
                string TokenLiteral() override;

                /**
                 * 获取 Program 的String
                 * @return
                 */
                string String() override;

        };
        
    } // namespace ast
    


} // namespace monkey


#endif //MONKEY_PROGRAM_H