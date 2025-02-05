#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <fstream> 
#include <memory>


namespace AST {

enum class TreeNodeType {
    kWhileNode        = 0,
    kIfNode           = 1,
    kElseNode         = 2,
    kDeclNode         = 3,
    kAssignmentNode   = 4,
    kValueNode        = 5,
    kPrintNode        = 6,
    kBinOpNode        = 7,
    kQuestionMarkNode = 8,
    kScopeNode        = 9,
};

class TreeNode {
  private:
    TreeNodeType node_type_;
  public:
    TreeNode(TreeNodeType node_type) : node_type_{node_type} {}
    virtual ~TreeNode() {}
    virtual void print(std::ofstream& file) const = 0;

  public: 
    TreeNodeType get_type() const {
      return node_type_;
    }
};

class WhileNode final : public TreeNode {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
  public:
    WhileNode(TreeNode* condition_node, TreeNode* scope_node) 
        : TreeNode{TreeNodeType::kWhileNode}, condition_node_{condition_node}, scope_node_{scope_node} {}
    ~WhileNode() {}

    void print(std::ofstream& file) const override {
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
}; 

class IfNode final : public TreeNode {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
    TreeNode* else_node_;
  public:
    IfNode(TreeNode* condition_node, TreeNode* scope_node, TreeNode* else_node) 
        : TreeNode{TreeNodeType::kIfNode}, condition_node_{condition_node}, scope_node_{scope_node}, else_node_{else_node} {}
    ~IfNode() {}

    void print(std::ofstream& file) const override {
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
};

class ElseNode final: public TreeNode {
  private:
    TreeNode* scope_node_;
  public:
    ElseNode(TreeNode* scope_node) 
        : TreeNode{TreeNodeType::kElseNode}, scope_node_{scope_node} {}
    ~ElseNode() {}

    void print(std::ofstream& file) const override {
      file << "\t\"" << this << "\" [label = \"Else node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";

      if (scope_node_) {
        file << "\t\"" << this << "\" -> \"" << scope_node_ << "\"" << std::endl;
        scope_node_->print(file);
      }
    }
 };

class DeclNode final : public TreeNode {
  private:
    std::string variable_name_;
    int value_ = 0;
  public:
    DeclNode(const std::string& variable_name) 
        : TreeNode{TreeNodeType::kDeclNode}, variable_name_{variable_name} {}
    ~DeclNode() {};

    void print(std::ofstream& file) const override {
      file << "\t\"" << this << "\" [label = \"Decl node (name of variable: " << variable_name_ << ")\", shape = \"octagon\", style = \"filled\", fillcolor = \"#95c9bc\"]";
    }
};

class AssignmentNode final : public TreeNode {
  private:
    TreeNode* decl_node_;
    TreeNode* expr_node_;
  public:
    AssignmentNode(TreeNode* decl_node = nullptr, TreeNode* expr_node = nullptr)
        : TreeNode{TreeNodeType::kAssignmentNode}, decl_node_{decl_node}, expr_node_{expr_node} {}
    ~AssignmentNode() {};

    void print(std::ofstream& file) const override {
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

};

class ValueNode final : public TreeNode {
  private:
    int value_;
  public:
    ValueNode(int value)
        : TreeNode{TreeNodeType::kValueNode}, value_{value} {}
    ~ValueNode() {}

    void print(std::ofstream& file) const override {
      file << "\t\"" << this << "\" [label = \"Value node: " << value_ << "\", shape = \"octagon\", style = \"filled\", fillcolor = \"#a4c995\"]";
    }
};

class PrintNode final : public TreeNode {
  private:
    TreeNode* node_to_print_;
  public:
    PrintNode(TreeNode* node_to_print = nullptr)
        : TreeNode{TreeNodeType::kPrintNode}, node_to_print_{node_to_print} {}

    ~PrintNode() {}

    void print(std::ofstream& file) const override {
      file << "\t\"" << this << "\" [label = \"Print node\", shape = \"box\", style = \"filled\", fillcolor = \"#e00a07\"]";

      if (node_to_print_) {
        file << "\t\"" << this << "\" -> \"" << node_to_print_ << "\"" << std::endl;
        node_to_print_->print(file);
      }
    }

};

enum class BinaryOpType {
    kAdd = 0,
    kSub = 1,
    kMul = 2,
    kDiv = 3,
};

class BinOpNode final : public TreeNode {
  private:
    BinaryOpType bin_op_;
    TreeNode* left_operand_;
    TreeNode* right_operand_;
  public:
    BinOpNode(BinaryOpType bin_op, TreeNode* left_operand = nullptr, TreeNode* right_operand = nullptr)
        : TreeNode{TreeNodeType::kBinOpNode}, bin_op_{bin_op}, left_operand_{left_operand}, right_operand_{right_operand} {}
    ~BinOpNode() {}

    void print(std::ofstream& file) const override {
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
};

enum class LogicalOpType {
    kEqual          = 0,
    kNotEqual       = 1,
    kBelow          = 2,
    kEqualOrBelow   = 3,
    kGreater        = 4,
    kEqualOrGreater = 5,
};

class LogOpNode final : public TreeNode {
  private:
    LogicalOpType log_op_;
    TreeNode* left_operand_;
    TreeNode* right_operand_;
  public:
    LogOpNode(LogicalOpType log_op, TreeNode* left_operand = nullptr, TreeNode* right_operand = nullptr)
        : TreeNode{TreeNodeType::kBinOpNode}, log_op_{log_op}, left_operand_{left_operand}, right_operand_{right_operand} {}
    ~LogOpNode() {}

    void print(std::ofstream& file) const override {
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
};

class QuestionMarkNode final : public TreeNode {
  private:
    TreeNode* decl_node_;
  public:
    QuestionMarkNode(TreeNode* decl_node = nullptr)
        : TreeNode{TreeNodeType::kQuestionMarkNode}, decl_node_{decl_node} {}
    ~QuestionMarkNode() {}

    void print(std::ofstream& file) const override {
      file << "\t\"" << this << "\" [label = \"? node\", shape = \"box\", style = \"filled\", fillcolor = \"#1f77b4\"]";

      if (decl_node_) {
        file << "\t\"" << this << "\" -> \"" << decl_node_ << "\"" << std::endl;
        decl_node_->print(file);
      }
    } 
};

class ScopeNode final : public TreeNode {
  private:
    ScopeNode* parent_node_;
    std::vector<TreeNode*> nodes_;

  public:
    ScopeNode(ScopeNode* parent_node)
        : TreeNode{TreeNodeType::kScopeNode}, parent_node_{parent_node} {};
    ~ScopeNode() {}

    void print(std::ofstream& file) const override {
      if (parent_node_)
        file << "\t\"" << this << "\" [label = \"Scope\", shape = \"circle\"]";
      else 
        file << "\t\"" << this << "\" [label = \"Global scope\", shape = \"circle\"]";
      for (auto it : nodes_)
        if (it) {
          file << "\t\"" << this << "\" -> \"" << it << "\"" << std::endl;
          it->print(file);
        }
    }

    void add_node(TreeNode* new_node) {
      nodes_.push_back(new_node);
    }

    ScopeNode* close_scope() {
      return parent_node_;
    }
};

class Ast final {
  public:
    TreeNode* root_= nullptr;
    std::vector<TreeNode*> nodes_{};

  public:
    bool empty() const {
      return nodes_.empty();
    }

    void print(std::ofstream& file) const {

      file.open("../graphviz/graph.dot");
      file << "digraph G{" << std::endl;
      if (root_) {
        root_->print(file);
      }
      file << "}";  
      file.close();
      char call_graph[100] = " ";
      snprintf(call_graph, 100, "dot ../graphviz/graph.dot -Tpng -o ../graphviz/graph.png");
      system(call_graph);
    }

    WhileNode* insert_while_node(TreeNode* condition_node = nullptr, TreeNode* scope_node = nullptr) {
      WhileNode* new_while_node = new WhileNode{condition_node, scope_node};
      nodes_.push_back(new_while_node);
      return static_cast<WhileNode*>(nodes_.back());
    }

    IfNode* insert_if_node(TreeNode* condition_node = nullptr, TreeNode* scope_node = nullptr, TreeNode* else_node = nullptr) {
      IfNode* new_if_node = new IfNode{condition_node, scope_node, else_node};
      nodes_.push_back(new_if_node);
      return static_cast<IfNode*>(nodes_.back());
    }

    ElseNode* insert_else_node(TreeNode* scope_node = nullptr) {
      ElseNode* new_else_node = new ElseNode{scope_node};
      nodes_.push_back(new_else_node);
      return static_cast<ElseNode*>(nodes_.back());
    }

    DeclNode* insert_decl_node(const std::string& variable_name) {
      DeclNode* new_decl_node = new DeclNode{variable_name};
      nodes_.push_back(new_decl_node);
      return static_cast<DeclNode*>(nodes_.back());
    }

    AssignmentNode* insert_assignment_node(TreeNode* decl_node = nullptr, TreeNode* expr_node = nullptr) {
      AssignmentNode* new_assignment_node = new AssignmentNode{decl_node, expr_node};
      nodes_.push_back(new_assignment_node);
      return static_cast<AssignmentNode*>(nodes_.back());
    }

    ValueNode* insert_value_node(int value) {
      ValueNode* new_value_node = new ValueNode{value};
      nodes_.push_back(new_value_node);
      return static_cast<ValueNode*>(nodes_.back());
    }

    PrintNode* insert_print_node(TreeNode* left_node = nullptr) {
      PrintNode* new_print_node = new PrintNode{left_node};
      nodes_.push_back(new_print_node);
      return static_cast<PrintNode*>(nodes_.back());
    }

    BinOpNode* insert_bin_op_node(BinaryOpType bin_op, TreeNode* left_operand = nullptr, TreeNode* right_operand = nullptr) {
      BinOpNode* new_bin_op_node = new BinOpNode{bin_op, left_operand, right_operand};
      nodes_.push_back(new_bin_op_node);
      return static_cast<BinOpNode*>(nodes_.back());
    }

    LogOpNode* insert_log_op_node(LogicalOpType log_op, TreeNode* left_operand = nullptr, TreeNode* right_operand = nullptr) {
      LogOpNode* new_log_op_node = new LogOpNode{log_op, left_operand, right_operand};
      nodes_.push_back(new_log_op_node);
      return static_cast<LogOpNode*>(nodes_.back());
    }

    QuestionMarkNode* insert_question_mark_node(TreeNode* decl_node = nullptr) {
      QuestionMarkNode* new_question_mark_node = new QuestionMarkNode(decl_node);
      nodes_.push_back(new_question_mark_node);
      return static_cast<QuestionMarkNode*>(nodes_.back());
    }

    ScopeNode* insert_scope_node(ScopeNode* parent_node = nullptr) {
      ScopeNode* new_scope_node = new ScopeNode{parent_node};
      nodes_.push_back(new_scope_node);
      return static_cast<ScopeNode*>(nodes_.back());
    }
};


} // namespace AST

#endif // AST_HPP
