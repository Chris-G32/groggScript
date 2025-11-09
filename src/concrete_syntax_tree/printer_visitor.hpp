#pragma once
#include <iostream>
#include <ostream>

#include "alphabet/abstract_alphabet_node.hpp"
#include "alphabet/binary_expression.hpp"
#include "alphabet/function_declaration.hpp"
#include "alphabet/literal.hpp"
#include "alphabet/program.hpp"
#include "alphabet/statement.hpp"
#include "alphabet/statements.hpp"
#include "alphabet/term.hpp"
#include "alphabet/unary_expression.hpp"
using namespace GSAlphabet;
class PrinterVisitor : public AbstractAlphabetNodeVisitor {
   public:
    /**
     * Creates a printer visitor for an AST node
     * @param tabSize Number of spaces per tab in output
     */
    explicit PrinterVisitor(int tabSize = 2) : _tabSize(tabSize) {}
    void visit(AbstractAlphabetNode* node) override {
        desc();
        AbstractAlphabetNodeVisitor::visit(node);
        asc();
    }
    void visitProgram(Program* node) override {
        printNodeName("program");
        visit(node->statements.get());
    }
    void visitStatements(Statements* node) override {
        printNodeName("statements");
        for (const auto& stmt : node->statements) {
            visit(stmt.get());
        }
    }
    void visitStatement(Statement* node) override {
        printNodeName("statement");
        printChild("child", node->child);
    }
    void visitVariableDeclaration(VariableDeclaration* node) override {
        printNodeName("variable_declaration");
        if (node->type) {
            printChild("type", *node->type);
        }
        printChild("identifier", node->identifier);
        if (node->initializer != nullptr) {
            printChild("initializer", node->initializer);
        }
    }
    void visitVariableAssignment(VariableAssignment* node) override {
        printNodeName("variable_assignment");
        printChild("not_implemented", "not implemented");
    }
    void visitBinaryExpression(BinaryExpression* node) override {
        printNodeName("binary_expression");
        printChild("left", node->left);
        printChild("operator", toString(node->op));
        printChild("right", node->right);
    };
    void visitUnaryExpression(UnaryExpression* node) override {
        printNodeName("unary_expression");
        printChild("child", node->node);
        printChild("operation", toString(node->op));
    }

    void visitSymbol(Symbol* node) override {
        printNodeName("symbol");
        printChild("identifier", node->identifier);
    };
    void visitLiteral(Literal* node) override {
        printNodeName("literal");
        std::visit(
            [this](const auto& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, decimal>) {
                    printChild("type", "float");
                } else if constexpr (std::is_same_v<T, text>) {
                    printChild("type", "string");
                } else if constexpr (std::is_same_v<T, integer>) {
                    printChild("type", "integer");
                } else if constexpr (std::is_same_v<T, boolean>) {
                    printChild("type", "boolean");
                } else {
                    printChild("type", "unknown");
                    printChild("value", "unknown");
                    return;
                }
                printChild("value", to_string(arg));
            },
            node->literal);
    }
    void visitCallExpression(CallExpression* node) override {
        printNodeName("call_expression");
        printChild("callee", node->callee);
        auto count = 1;
        for (auto& arg : node->arguments) {
            printChild("arg" + std::to_string(count++), arg);
        }
    }
    void visitTerm(Term* node) override {
        printNodeName("term");
        visit(node->term.get());
    };
    void visitFunctionDeclaration(FunctionDeclaration* node) override {
        printNodeName("function_declaration");
        printChild("body", node->body);
        printTabDepth();
        std::cout << "parameters\n";
        desc();
        for (const auto& arg : node->arguments) {
            printChild("name", arg.name);
            printChild("type", arg.type);
        }
        asc();
        printChild("return_type",
                   node->returnType.value_or("no_type_provided"));
    }

   private:
    int _tabSize;
    void printNodeName(const std::string& name) {
        printTabDepth();
        std::cout << name << '\n';
    }
    // Used for printing primitives, like operators, and symbol valuess
    void printChild(const std::string& name, const std::string& val) {
        desc();
        printTabDepth();
        std::cout << name << '\n';
        desc();
        printTabDepth();
        std::cout << val << std::endl;
        asc();
        asc();
    }
    void printChild(const std::string& name,
                    std::unique_ptr<AbstractAlphabetNode>& node) {
        desc();
        printTabDepth();
        std::cout << name << '\n';
        visit(node.get());
        asc();
    }
    void desc() { depth++; }
    void asc() { depth--; }
    void printTabDepth() const {
        // Can overflow but we dont care :)
        for (int i = 0; i < (depth * _tabSize); i++) {
            std::cout << ' ';
        }
    }

    // Init to this so first program call results in no indentation
    int depth = -1;
};