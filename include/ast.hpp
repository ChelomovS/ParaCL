#ifndef TREE_HPP_
#define TREE_HPP_

#include <string>
#include <vector>

namespace Ast {

enum class TreeNodeType {
    kWhileNode        = 0,
    kIfNode           = 1,
    kDeclNode         = 2,
    kAssignmentNode   = 3,
    kValueNode        = 4,
    kPrintNode        = 5,
    kBinOpNode        = 6,
    kQuestionMarkNode = 7,
    kScopeNode        = 8,
};

class TreeNode {
  private:
    TreeNode* parent_;
    TreeNodeType node_type_;
  public:
    TreeNode(TreeNode* parent, TreeNodeType node_type) 
        : parent_(parent), node_type_(node_type) {}
    virtual ~TreeNode() = 0;
};

class Ast {
    TreeNode* root;
};

class WhileNode : public TreeNode {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
  public:
    WhileNode(TreeNode* parent, TreeNode* condition_node, TreeNode* scope_node) 
        : TreeNode{parent, TreeNodeType::kWhileNode}, condition_node_(condition_node), scope_node_(scope_node) {}
    ~WhileNode() { condition_node_->~TreeNode(); scope_node_->~TreeNode(); }
}; 

class IfNode : public TreeNode {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
  public:
    IfNode(TreeNode* parent, TreeNode* condition_node, TreeNode* scope_node) 
        : TreeNode{parent, TreeNodeType::kIfNode}, condition_node_(condition_node), scope_node_(scope_node) {}
    ~IfNode() { condition_node_->~TreeNode(); scope_node_->~TreeNode(); }
};

class ExpressionNode : public TreeNode {
  private:
    // FIXME это вообще пиздец описывать
  public:

};

class DeclNode : public TreeNode {
  private:
    std::string variable_name_;
  public:
    DeclNode(TreeNode* parent, const std::string& variable_name) 
        : TreeNode{parent, TreeNodeType::kDeclNode}, variable_name_(variable_name) {}
    ~DeclNode() {};
};

class AssignmentNode : public TreeNode {
  private:
    TreeNode* decl_node_;
    TreeNode* expr_node_;
  public:
    AssignmentNode(TreeNode* parent, TreeNode* decl_node, TreeNode* expr_node)
        : TreeNode{parent, TreeNodeType::kAssignmentNode}, decl_node_(decl_node), expr_node_(expr_node) {}
    ~AssignmentNode() { decl_node_->~TreeNode(); expr_node_->~TreeNode(); };
};

class ValueNode : public TreeNode {
  private:
    // NOTE should be int?
    int value_;
  public:
    ValueNode(TreeNode* parent, int value)
        : TreeNode{parent, TreeNodeType::kValueNode}, value_(value) {}
    ~ValueNode() {}
};

class PrintNode : public TreeNode {
  private:
    TreeNode* decl_node_;
  public:
    PrintNode(TreeNode* parent, TreeNode* decl_node)
        : TreeNode{parent, TreeNodeType::kPrintNode}, decl_node_(decl_node) {}
    ~PrintNode() { decl_node_->~TreeNode(); }
};

enum class BinaryOpType {
    kAdd = 0,
    kSub = 1,
};

class BinOpNode : public TreeNode {
  private:
    BinaryOpType bin_op_;
    TreeNode* left_operand_;
    TreeNode* right_operand_;
  public:
    BinOpNode(TreeNode* parent, BinaryOpType bin_op, TreeNode* left_operand, TreeNode* right_operand)
        : TreeNode{parent, TreeNodeType::kBinOpNode}, bin_op_(bin_op), left_operand_(left_operand), right_operand_(right_operand) {}
    ~BinOpNode() { left_operand_->~TreeNode(); right_operand_->~TreeNode(); }
};

class QuestionMarkNode : public TreeNode {
  private:
  public:
    QuestionMarkNode(TreeNode* parent)
        : TreeNode{parent, TreeNodeType::kQuestionMarkNode} {}
    ~QuestionMarkNode() {}
};

class ScopeNode : public TreeNode {
  private:
    std::vector<TreeNode*> scope_statements_; // NOTE ?
  public:
    ScopeNode(TreeNode* parent, const std::vector<TreeNode*>& scope_statements)
        : TreeNode{parent, TreeNodeType::kScopeNode}, scope_statements_(scope_statements) {}
    ~ScopeNode() {
        for (auto it_statements: scope_statements_) {
            it_statements->~TreeNode();
        }
    }
};

} // namespace Ast

#endif // TREE_HPP_
