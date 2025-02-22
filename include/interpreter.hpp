#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>
#include <utility>

#include <FlexLexer.h>

#include "ast.hpp"
#include "entity_table.hpp"
#include "node_visitor.hpp"

namespace intpr {

using IntprInt = int;

class Interpreter final : public ast::NodeVisitor {
  private:
    ast::Ast ast_;
    EntityTable<IntprInt> entity_table;
    std::vector<IntprInt> eval_stack;
  public:
    Interpreter(ast::Ast ast) : ast_{std::move(ast)} {}

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
