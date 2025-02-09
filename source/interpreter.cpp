#include "interpreter.hpp"

#include <ostream>

#include "ast.hpp"

namespace intpr {

void Interpreter::visit_all() {
    ast_.root_->accept(this);
}

void Interpreter::visit(const ast::WhileNode& node) {
   
}

void Interpreter::visit(const ast::IfNode& node) {
    // node.get_cond();
}

void Interpreter::visit(const ast::ElseNode& node) {

}

void Interpreter::visit(const ast::DeclNode& node) {
    // do nothing 
}

void Interpreter::visit(const ast::AssignmentNode& node) {
    
}

void Interpreter::visit(const ast::ValueNode& node) {
    eval_stack.push_back(node.get_value());
}

void Interpreter::visit(const ast::PrintNode& node) {
    const ast::ExprNode* expr_node = static_cast<const ast::ExprNode*>(node.get_to_print());
    expr_node->accept(this);

    int value_to_print = eval_stack.back();
    eval_stack.pop_back();

    std::cout << "Print: " << value_to_print << std::endl;
}

void Interpreter::visit(const ast::BinOpNode& node) {

}

void Interpreter::visit(const ast::LogOpNode& node) {

}

void Interpreter::visit(const ast::QuestionMarkNode& node) {

}

void Interpreter::visit(const ast::ScopeNode& node) {

}

void Interpreter::visit(const ast::ExprNode& node) {
    const ast::TreeNode* expr_node = node.get_expr();
    expr_node->accept(this);
}

} // namespace intpr
