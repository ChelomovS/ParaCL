#ifndef GRAPH_DUMP_HPP
#define GRAPH_DUMP_HPP

#include <ostream>
#include <filesystem>
#include <memory>
#include <fstream>

#include "parser/ast.hpp"
#include "parser/node_visitor.hpp"

namespace gdump {

class GraphDump final : public ast::NodeVisitor {
  private:
    std::unique_ptr<std::ostream> dump_file_;
  public:
    GraphDump(const ast::Ast& ast) : NodeVisitor{ast}, dump_file_{nullptr} {
        std::filesystem::path graphviz_dir = "graphviz";
        if (!std::filesystem::exists(graphviz_dir)) {
            std::filesystem::create_directory(graphviz_dir);
        }

        std::filesystem::path dot_file = graphviz_dir / "graph.dot";
        dump_file_ = std::make_unique<std::ofstream>(dot_file);
    }

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

} // namespace gdump

#endif // GRAPH_DUMP_HPP

