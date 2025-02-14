#include "ast.hpp"

#include <iostream>
#include <filesystem>

#include <spdlog/spdlog.h>

namespace ast {

void WhileNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"While node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";

    if (condition_node_) {
        file << "\t\"" << this << "\" -> \"" << condition_node_ << "\"" << std::endl;
        condition_node_->print(file);
    }

    if (scope_node_) {
        file << "\t\"" << this << "\" -> \"" << scope_node_ << "\"" << std::endl;
        scope_node_->print(file);
    }
}

void IfNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"If node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";

    if (condition_node_) {
        file << "\t\"" << this << "\" -> \"" << condition_node_ << "\"" << std::endl;
        condition_node_->print(file);
    }

    if (scope_node_) {
        file << "\t\"" << this << "\" -> \"" << scope_node_ << "\"" << std::endl;
        scope_node_->print(file);
    }

    if (else_node_) {
        file << "\t\"" << this << "\" -> \"" << else_node_ << "\"" << std::endl;
        else_node_->print(file);
    }
}

void ElseNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"Else node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";

    if (scope_node_) {
        file << "\t\"" << this << "\" -> \"" << scope_node_ << "\"" << std::endl;
        scope_node_->print(file);
    }
}

void DeclNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"Decl node (name of variable: " << variable_name_ << ")\", shape = \"octagon\", style = \"filled\", fillcolor = \"#95c9bc\"]";
}

void VarDerefNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"VarDeref node (name of variable: " << variable_name_ << ")\", shape = \"octagon\", style = \"filled\", fillcolor = \"#95c9bc\"]";
}

void AssignmentNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"Assignment node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";

    if (decl_node_) {
        file << "\t\"" << this << "\" -> \"" << decl_node_ << "\"" << std::endl;
        decl_node_->print(file);
    }

    if (expr_node_) {
        file << "\t\"" << this << "\" -> \"" << expr_node_ << "\"" << std::endl;
        expr_node_->print(file);
    }
}

void ValueNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"Value node: " << value_ << "\", shape = \"octagon\", style = \"filled\", fillcolor = \"#a4c995\"]";
}

void PrintNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"Print node\", shape = \"box\", style = \"filled\", fillcolor = \"#e00a07\"]";

    if (node_to_print_) {
        file << "\t\"" << this << "\" -> \"" << node_to_print_ << "\"" << std::endl;
        node_to_print_->print(file);
    }
}

void BinOpNode::print(std::ofstream& file) const {
    switch (bin_op_) {
        case BinaryOpType::kAdd:
            file << "\t\"" << this << "\" [label = \"+ node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
        case BinaryOpType::kSub:
            file << "\t\"" << this << "\" [label = \"- node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
        case BinaryOpType::kMul:
            file << "\t\"" << this << "\" [label = \"* node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
        case BinaryOpType::kDiv:
            file << "\t\"" << this << "\" [label = \"/ node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
    }

    if (left_operand_) {
        file << "\t\"" << this << "\" -> \"" << left_operand_ << "\"" << std::endl;
        left_operand_->print(file);
    }

    if (right_operand_) {
        file << "\t\"" << this << "\" -> \"" << right_operand_ << "\"" << std::endl;
        right_operand_->print(file);
    }
}

void LogOpNode::print(std::ofstream& file) const {
    switch (log_op_) {
        case LogicalOpType::kEqual:
            file << "\t\"" << this << "\" [label = \"== node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
        case LogicalOpType::kNotEqual:
            file << "\t\"" << this << "\" [label = \"!= node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
        case LogicalOpType::kBelow:
            file << "\t\"" << this << "\" [label = \"< node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
        case LogicalOpType::kEqualOrBelow:
            file << "\t\"" << this << "\" [label = \"<= node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
        case LogicalOpType::kGreater:
            file << "\t\"" << this << "\" [label = \"> node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
        case LogicalOpType::kEqualOrGreater:
            file << "\t\"" << this << "\" [label = \">= node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
            break;
    }

    if (left_operand_) {
        file << "\t\"" << this << "\" -> \"" << left_operand_ << "\"" << std::endl;
        left_operand_->print(file);
    }

    if (right_operand_) {
        file << "\t\"" << this << "\" -> \"" << right_operand_ << "\"" << std::endl;
        right_operand_->print(file);
    }
}

void QuestionMarkNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"? node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";
} 

void ScopeNode::print(std::ofstream& file) const {
    if (parent_node_) {
        file << "\t\"" << this << "\" [label = \"Scope\", shape = \"circle\"]";
    } else {
        file << "\t\"" << this << "\" [label = \"Global scope\", shape = \"circle\"]";
    }

    for (auto it : nodes_) {
        if (it) {
            file << "\t\"" << this << "\" -> \"" << it << "\"" << std::endl;
            it->print(file);
        }
    }
}

void ExprNode::print(std::ofstream& file) const {
    file << "\t\"" << this << "\" [label = \"expr node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";

    if (expr_node_ != nullptr) {
        file << "\t\"" << this << "\" -> \"" << expr_node_ << "\"" << std::endl;
        expr_node_->print(file);
    }
}

void Ast::print() const {
    std::filesystem::path graphviz_dir = "../graphviz";
    if (!std::filesystem::exists(graphviz_dir)) {
        std::filesystem::create_directory(graphviz_dir);
    }

    std::filesystem::path dot_file = graphviz_dir / "graph.dot";
    {
        std::ofstream file(dot_file);

        file << "digraph G{" << std::endl;
        if (root_) {
            root_->print(file);
        }
        file << "}";
    }
    
    std::filesystem::path image_file = graphviz_dir / "graph.png";
    std::string command = "dot " + dot_file.string() + " -Tpng -o " + image_file.string();

    spdlog::debug("command for system: {}", command);
    int sys_res = system(command.c_str());
    spdlog::debug("system on graphviz: {}", sys_res);
}

} // namespace ast
