#include "graph_dump/graph_dump.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <cassert>

#include <spdlog/spdlog.h>

#include "parser/ast.hpp"

inline std::string format_node(const std::string& node_type, const std::string& color) {
    return "\" [label = \"" + node_type + "\", shape = \"box\", style = \"filled\", fillcolor = \"" + color + "\"]";
}

namespace gdump {

void GraphDump::visit_all() {
    *dump_file_ << "digraph G {" << std::endl;
    if (ast_->root_) {
        ast_->root_->accept(this);
    }

    *dump_file_ << "}" << std::endl;

    std::filesystem::path graphviz_dir = "graphviz";
    if (!std::filesystem::exists(graphviz_dir)) {
        std::filesystem::create_directory(graphviz_dir);
    }
    std::filesystem::path image_file = graphviz_dir / "graph.png";
    std::filesystem::path dot_file = graphviz_dir / "graph.dot";
    std::string command = "dot " + dot_file.string() + " -Tpng -o " + image_file.string();

    spdlog::debug("command for system: {}", command);
    int sys_res = std::system(command.c_str());
    spdlog::debug("system on graphviz: {}", sys_res);
}

void GraphDump::visit(const ast::WhileNode& node) {
    const ast::TreeNode* cond_node = node.get_cond(); 
    const ast::TreeNode* scope_node = node.get_scope();

    *dump_file_ << "\t\"" << &node << format_node("While", "#1f77b4") << std::endl;

    if (cond_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << cond_node << "\"" << std::endl;
        cond_node->accept(this);
    }

    if (scope_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << scope_node << "\"" << std::endl;
        scope_node->accept(this);
    }
}

void GraphDump::visit(const ast::IfNode& node) {
    const ast::TreeNode* cond_node = node.get_cond();
    const ast::TreeNode* scope_node = node.get_scope();
    const ast::TreeNode* else_node = node.get_else();

    *dump_file_ << "\t\"" << &node << format_node("If", "#1f77b4") << std::endl;

    if (cond_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << cond_node << "\"" << std::endl;
        cond_node->accept(this);
    }

    if (scope_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << scope_node << "\"" << std::endl;
        scope_node->accept(this);
    }

    if (else_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << else_node << "\"" << std::endl;
        else_node->accept(this);
    }
}

void GraphDump::visit(const ast::ElseNode& node) {
    const ast::TreeNode* scope_node = node.get_scope();
   
    *dump_file_ << "\t\"" << &node << format_node("Else", "#1f77b4") << std::endl;

    if (scope_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << scope_node << "\"" << std::endl;
        scope_node->accept(this);
    }
}

void GraphDump::visit(const ast::DeclNode& node) {
    std::string variable_name = node.get_name();

    *dump_file_ << "\t\"" << &node << format_node("Decl (" + variable_name + ")", "#95c9bc") << std::endl;
}

void GraphDump::visit(const ast::VarDerefNode& node) {
    std::string variable_name = node.get_name();

    *dump_file_ << "\t\"" << &node << format_node("VarDeref (" + variable_name + ")", "#95c9bc") << std::endl;
}

void GraphDump::visit(const ast::AssignmentNode& node) {
    const ast::DeclNode* decl_node = static_cast<const ast::DeclNode*>(node.get_decl());
    const ast::TreeNode* expr_node = node.get_expr();

    *dump_file_ << "\t\"" << &node << format_node("Assignment", "#1f77b4") << std::endl;

    if (decl_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << decl_node << "\"" << std::endl;
        decl_node->accept(this);
    }

    if (expr_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << expr_node << "\"" << std::endl;
        expr_node->accept(this);
    }
}

void GraphDump::visit(const ast::ValueNode& node) {
    *dump_file_ << "\t\"" << &node << format_node("Value (" + std::to_string(node.get_value()) + ")", "#a4c995") << std::endl;
}

void GraphDump::visit(const ast::PrintNode& node) {
    const ast::TreeNode* expr_node = node.get_to_print();
 
    *dump_file_ << "\t\"" << &node << format_node("Print", "#e00a07") << std::endl;

    if (expr_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << expr_node << "\"" << std::endl;
        expr_node->accept(this);
    }
}

void GraphDump::visit(const ast::BinOpNode& node) {
    const ast::TreeNode* left_node = node.get_left_op();
    const ast::TreeNode* right_node = node.get_right_op();
    ast::BinaryOpType bin_op = node.get_bin_op();

    std::string op_str = "";
    switch (bin_op) {
        case ast::BinaryOpType::kAdd:       op_str = "+"; break;
        case ast::BinaryOpType::kSub:       op_str = "-"; break;
        case ast::BinaryOpType::kMul:       op_str = "*"; break;
        case ast::BinaryOpType::kDiv:       op_str = "/"; break;
        case ast::BinaryOpType::kModulus:   op_str = "%"; break;
        default:
            assert(0 && "unknown bin op");
    }
    *dump_file_ << "\t\"" << &node << format_node(op_str, "#1f77b4") << std::endl;

    if (left_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << left_node << "\"" << std::endl;
        left_node->accept(this);
    }

    if (right_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << right_node << "\"" << std::endl;
        right_node->accept(this);
    }
}

void GraphDump::visit(const ast::UnOpNode& node) {
    const ast::TreeNode* operand = node.get_operand();
    ast::UnaryOpType unary_op = node.get_unary_op();

    std::string op_str = "";
    switch (unary_op) {
        case ast::UnaryOpType::kNot:   op_str = "!"; break;
        case ast::UnaryOpType::kMinus: op_str = "-"; break;
        case ast::UnaryOpType::kPlus:  op_str = "+"; break;
        default:
            assert(0 && "unknown un op");
    }
    *dump_file_ << "\t\"" << &node << format_node(op_str, "#1f77b4") << std::endl;

    if (operand) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << operand << "\"" << std::endl;
        operand->accept(this);
    }
}

void GraphDump::visit(const ast::LogOpNode& node) {    
    const ast::TreeNode* left_node = node.get_left_op();
    const ast::TreeNode* right_node = node.get_right_op();
    ast::LogicalOpType logical_op = node.get_logical_op();

    std::string op_str = "";
    switch (logical_op) {
        case ast::LogicalOpType::kEqual:            op_str = "=="; break;
        case ast::LogicalOpType::kNotEqual:         op_str = "!="; break;
        case ast::LogicalOpType::kBelow:            op_str = "<" ; break;
        case ast::LogicalOpType::kEqualOrBelow:     op_str = "<="; break;
        case ast::LogicalOpType::kGreater:          op_str = ">" ; break;
        case ast::LogicalOpType::kEqualOrGreater:   op_str = ">="; break;
        case ast::LogicalOpType::kAnd:              op_str = "&&"; break;
        case ast::LogicalOpType::kOr:               op_str = "||"; break;
        case ast::LogicalOpType::kXor:              op_str = "^" ; break;
        default:
            assert(0 && "unknown logical op");
    }
    *dump_file_ << "\t\"" << &node << format_node(op_str, "#1f77b4") << std::endl;

    if (left_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << left_node << "\"" << std::endl;
        left_node->accept(this);
    }

    if (right_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << right_node << "\"" << std::endl;
        right_node->accept(this);
    }
}

void GraphDump::visit(const ast::QuestionMarkNode& node) {
    (void)node;

    *dump_file_ << "\t\"" << &node << format_node("?", "#1f77b4") << std::endl;
}

void GraphDump::visit(const ast::ScopeNode& node) {
    const ast::TreeNode* parent_node = node.get_parent_scope();
    if (parent_node) {
        *dump_file_ << "\t\"" << &node << format_node("Scope", "#ffffff") << std::endl;
    } else {
        *dump_file_ << "\t\"" << &node << format_node("Global scope", "#ffffff") << std::endl;
    }

    auto nodes = node.get_nodes();
    for (auto it_node : *nodes) {
        if (!it_node) {
            continue;
        }

        *dump_file_ << "\t\"" << &node << "\" -> \"" << it_node << "\"" << std::endl << std::endl;
        it_node->accept(this);
    }
}

void GraphDump::visit(const ast::ExprNode& node) {
    const ast::TreeNode* expr_node = node.get_expr();

    *dump_file_ << "\t\"" << &node << format_node("expr", "#1f77b4") << std::endl;

    if (expr_node) {
        *dump_file_ << "\t\"" << &node << "\" -> \"" << expr_node << "\"" << std::endl;
        expr_node->accept(this);
    }
}

}
