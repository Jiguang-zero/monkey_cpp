#include "Program.h"

#include <utility>

namespace monkey {
    namespace ast {
        string Program::TokenLiteral() {
            if (!Statements.empty()) {
                return Statements[0].TokenLiteral();
            }
            else {
                return "";
            }
        }

        void Program::setStatements(vector<Statement> statements) {
            Statements = std::move(statements);
        }

        vector<Statement>& Program::getStatements() {
            return Statements;
        }
    }

}