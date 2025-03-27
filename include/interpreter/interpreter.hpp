#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>

#include <FlexLexer.h>

#include "parser/ast.hpp"
#include "parser/node_visitor.hpp"
#include "interpreter/entity_table.hpp"

namespace intpr {

using IntprInt = int;

class Interpreter final : public ast::NodeVisitor {
  private:
    EntityTable<IntprInt> entity_table;
    std::vector<IntprInt> eval_stack;
  public:
    Interpreter(const ast::Ast* ast) : NodeVisitor{ast} {}

    void visit_all() override;
    void visit(const ast::WhileNode& node) override;
    void visit(const ast::IfNode& node) override;
    void visit(const ast::ElseNode& node) override;
    void visit(const ast::DeclNode& node) override;
    void visit(const ast::VarDerefNode& node) override;
    void visit(const ast::AssignmentNode& node) override;
    void visit(const ast::ValueNode& node) override;
    void visit(const ast::PrintNode& node) override;
    void visit(const ast::BinOpNode& node) override;
    void visit(const ast::UnOpNode& node) override;
    void visit(const ast::LogOpNode& node) override;
    void visit(const ast::QuestionMarkNode& node) override;
    void visit(const ast::ScopeNode& node) override;
    void visit(const ast::ExprNode& node) override;
};

} // namespace intpr

#endif // INTERPRETER_HPP
