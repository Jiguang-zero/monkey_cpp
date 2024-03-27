#include "Program.h"

namespace monkey {
    namespace ast {
        string Program::TokenLiteral() {
            if (Statements.size() > 0) {
                return Statements[0].TokenLiteral();
            }
            else {
                return "";
            }
        }

        void Program::setStatements(vector<Statement> statements) {
            Statements = statements;
        }

        vector<Statement> Program::getStatements() {
            return Statements;
        }
    }

}