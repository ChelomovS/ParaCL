#ifndef TREE_HPP_
#define TREE_HPP_

#include <string>
#include <vector>

#include "simulator.hpp"

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
    TreeNode(TreeNodeType node_type) : node_type_(node_type) {}
    virtual ~TreeNode() = 0;
  public: 
    TreeNodeType get_type() const {
      return node_type_;
    }
};

class Ast final {
    TreeNode* root_= nullptr;
    std::vector<TreeNode*> nodes_{};

  public:
    WhileNode* insert_while_node(TreeNode* condition_node, TreeNode* scope_node) {
      WhileNode* new_while_node = new WhileNode{condition_node, scope_node};
      nodes_.push_back(new_while_node);
      return new_while_node;
    }

    IfNode* insert_if_node(TreeNode* condition_node, TreeNode* scope_node, TreeNode* else_node) {
      IfNode* new_if_node = new IfNode{condition_node, scope_node, else_node};
      nodes_.push_back(new_if_node);
      return new_if_node;
    }

    ElseNode* insert_else_node(TreeNode* scope_node) {
      ElseNode* new_else_node = new ElseNode{scope_node};
      nodes_.push_back(new_else_node);
      return new_else_node;
    }

    DeclNode* insert_decl_node(const std::string& variable_name) {
      DeclNode* new_decl_node = new DeclNode{variable_name};
      nodes_.push_back(new_decl_node);
      return new_decl_node;
    }

    AssignmentNode* insert_assigment_node(TreeNode* decl_node, TreeNode* expr_node) {
      AssignmentNode* new_assignment_node = new AssignmentNode{decl_node, expr_node};
      nodes_.push_back(new_assignment_node);
      return new_assignment_node;
    }

    ValueNode* insert_value_node(int value) {
      ValueNode* new_value_node = new ValueNode{value};
      nodes_.push_back(new_value_node);
      return new_value_node;
    }

    PrintNode* insert_print_node(TreeNode* decl_node) {
      PrintNode* new_print_node = new TreeNode{decl_node};
      nodes_.push_back(new_print_node);
      return new_print_node;
    }

    BinOpNode* insert_bin_op_node(BinaryOpType bin_op, TreeNode* left_operand, TreeNode* right_operand) {
      BinOpNode* new_bin_op_node = new BinOpNode{bin_op, left_operand, right_operand};
      nodes_.push_back(new_bin_op_node);
      return new_bin_op_node;
    }

    void insert_log_op_node(LogicalOpType log_op, TreeNode* left_operand, TreeNode* right_operand) {
      LogOpNode* new_log_op_node = new LogOpNode{log_op, left_operand, right_operand};
      nodes_.push_back(new_log_op_node);
      return new_log_op_node;
    }

    void insert_question_mark_node(TreeNode* decl_node) {
      QuestionMarkNode* new_question_mark_node = new QuestionMarkNode{decl_node};
      nodes_.push_back(new_question_mark_node);
      return new_question_mark_node;
    }

    ScopeNode* insert_scope_node(const std::vector<TreeNode*>& scope_statements) {
      ScopeNode* new_scope_node = new ScopeNode{scope_statements};
      nodes_.push_back(new_scope_node);
      return new_scope_node;
    }
};

class WhileNode : public TreeNode final {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
  public:
    WhileNode(TreeNode* condition_node, TreeNode* scope_node) 
        : TreeNode{TreeNodeType::kWhileNode}, condition_node_(condition_node), scope_node_(scope_node) {}
    ~WhileNode() { condition_node_->~TreeNode(); scope_node_->~TreeNode(); }
  public: 
    calculate() {
      
    }
}; 

class IfNode : public TreeNode final {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
    TreeNode* else_node_;
  public:
    IfNode(TreeNode* condition_node, TreeNode* scope_node, TreeNode* else_node) 
        : TreeNode{parent, TreeNodeType::kIfNode}, condition_node_(condition_node), scope_node_(scope_node) {}
    ~IfNode() { condition_node_->~TreeNode(); scope_node_->~TreeNode(); else_node_->~ElseNode()}
};

class ElseNode : public TreeNode final {
  private:
    TreeNode* scope_node_;
  public:
    ElseNode(TreeNode* scope_node) 
        : TreeNode{TreeNodeType::kElseNode} scope_node_(scope_node) {}
    ~ElseNode() { scope_node_->~TreeNode(); }
};

class DeclNode : public TreeNode final {
  private:
    std::string variable_name_;
  public:
    DeclNode(const std::string& variable_name) 
        : TreeNode{TreeNodeType::kDeclNode}, variable_name_{variable_name} {}
    ~DeclNode() {};
};

class AssignmentNode : public TreeNode final {
  private:
    TreeNode* decl_node_;
    TreeNode* expr_node_;
  public:
    AssignmentNode(TreeNode* decl_node, TreeNode* expr_node)
        : TreeNode{TreeNodeType::kAssignmentNode}, decl_node_(decl_node), expr_node_(expr_node) {}
    ~AssignmentNode() { decl_node_->~TreeNode(); expr_node_->~TreeNode(); };
};

class ValueNode : public TreeNode final {
  private:
    int value_;
  public:
    ValueNode(int value)
        : TreeNode{TreeNodeType::kValueNode}, value_(value) {}

    int calculate() {
      return value_;
    }

    ~ValueNode() {}
};

class PrintNode : public TreeNode final {
  private:
    TreeNode* decl_node_;
  public:
    PrintNode(TreeNode* decl_node)
        : TreeNode{TreeNodeType::kPrintNode}, decl_node_(decl_node) {}
      
    void calculate() {
      print();
    }

    ~PrintNode() { decl_node_->~TreeNode(); }
};

enum class BinaryOpType {
    kAdd = 0,
    kSub = 1,
    kMul = 2,
    KDiv = 3,
};

class BinOpNode : public TreeNode final {
  private:
    BinaryOpType bin_op_;
    TreeNode* left_operand_;
    TreeNode* right_operand_;
  public:
    BinOpNode(BinaryOpType bin_op, TreeNode* left_operand, TreeNode* right_operand)
        : TreeNode{TreeNodeType::kBinOpNode}, bin_op_(bin_op), left_operand_(left_operand), right_operand_(right_operand) {}
    calculate()
    ~BinOpNode() { left_operand_->~TreeNode(); right_operand_->~TreeNode(); }
};

enum class LogicalOpType {
    kEqual          = 0,
    kNotEqual       = 1,
    kBelow          = 2,
    kEqualOrBelow   = 3,
    kGreater        = 4,
    kEqualOrGreater = 5,
};

class LogOpNode : public TreeNode final {
  private:
    LogicalOpType log_op_;
    TreeNode* left_operand_;
    TreeNode* right_operand_;
  public:
    LogOpNode(LogicalOpType log_op, TreeNode* left_operand, TreeNode* right_operand)
        : TreeNode{TreeNodeType::kBinOpNode}, log_op_(log_op), left_operand_(left_operand), right_operand_(right_operand) {}
    ~LogOpNode() { left_operand_->~TreeNode(); right_operand_->~TreeNode(); }
};

class QuestionMarkNode : public TreeNode final {
  private:
    TreeNode* decl_node_;
  public:
    QuestionMarkNode(TreeNode* decl_node)
        : TreeNode{TreeNodeType::kQuestionMarkNode},  decl_node_(decl_node) {}
    ~QuestionMarkNode() {}
};

class ScopeNode : public TreeNode final {
  private:
    std::vector<TreeNode*> scope_statements_; // NOTE ?
  public:
    ScopeNode(const std::vector<TreeNode*>& scope_statements)
        : TreeNode{TreeNodeType::kScopeNode}, scope_statements_(scope_statements) {}
    ~ScopeNode() {
        for (auto it_statements: scope_statements_) {
            it_statements->~TreeNode();
        }
    }
};

} // namespace Ast

#endif // TREE_HPP_
