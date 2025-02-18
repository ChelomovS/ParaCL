#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <deque>
#include <fstream> 
#include <vector>

#include "node_visitor.hpp"

namespace ast {

enum class TreeNodeType {
    kWhileNode        = 0,
    kIfNode           = 1,
    kElseNode         = 2,
    kDeclNode         = 3,
    kVarDerefNode     = 4,
    kAssignmentNode   = 5,
    kValueNode        = 6,
    kPrintNode        = 7,
    kBinOpNode        = 8,
    kUnOpNode         = 9,
    kQuestionMarkNode = 10,
    kScopeNode        = 11,
    kExprNode         = 12,
};

class WhileNode;
class IfNode;
class ElseNode;
class DeclNode;
class VarDerefNode;
class AssignmentNode;
class ValueNode;
class PrintNode;
class BinOpNode;
class UnOpNode;
class LogOpNode;
class ScopeNode;
class ExprNode;
class ExclamationMarkNode;

class TreeNode {
  private:
    TreeNodeType node_type_;
  public:
    TreeNode(TreeNodeType node_type) : node_type_{node_type} {}
    virtual ~TreeNode() = default;

    virtual void accept(NodeVisitor* visitor) const = 0;
    virtual void print(std::ofstream& file) const = 0;

    TreeNodeType get_type() const { return node_type_; }
};

class WhileNode final : public TreeNode {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
  public:
    virtual ~WhileNode() override = default;

    WhileNode(TreeNode* condition_node, TreeNode* scope_node) 
        : TreeNode{TreeNodeType::kWhileNode}, condition_node_{condition_node}, scope_node_{scope_node} {}


    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override; 
    
    const TreeNode* get_cond() const { return condition_node_; }
    const TreeNode* get_scope() const { return scope_node_; }
};

class IfNode final : public TreeNode {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
    TreeNode* else_node_;
  public:
    IfNode(TreeNode* condition_node, TreeNode* scope_node, TreeNode* else_node) 
        : TreeNode{TreeNodeType::kIfNode}, condition_node_{condition_node}, scope_node_{scope_node}, else_node_{else_node} {}
    virtual ~IfNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    const TreeNode* get_cond() const { return condition_node_; }
    const TreeNode* get_scope() const { return scope_node_; }
    const TreeNode* get_else() const { return else_node_; }
};

class ElseNode final: public TreeNode {
  private:
    TreeNode* scope_node_;
  public:
    ElseNode(TreeNode* scope_node) 
        : TreeNode{TreeNodeType::kElseNode}, scope_node_{scope_node} {}
    virtual ~ElseNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    const TreeNode* get_scope() const { return scope_node_; }
 };

class DeclNode final : public TreeNode {
  private:
    std::string variable_name_;
  public:
    DeclNode(const std::string& variable_name) 
        : TreeNode{TreeNodeType::kDeclNode}, variable_name_{variable_name} {}
    virtual ~DeclNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    std::string get_name() const { return variable_name_; }
};

class VarDerefNode final : public TreeNode {
  private:
    std::string variable_name_;
  public:
    VarDerefNode(const std::string& variable_name)
        : TreeNode{TreeNodeType::kVarDerefNode}, variable_name_{variable_name} {}
    virtual ~VarDerefNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    std::string get_name() const { return variable_name_; }
};

class AssignmentNode final : public TreeNode {
  private:
    TreeNode* decl_node_;
    TreeNode* expr_node_;
  public:
    AssignmentNode(TreeNode* decl_node = nullptr, TreeNode* expr_node = nullptr)
        : TreeNode{TreeNodeType::kAssignmentNode}, decl_node_{decl_node}, expr_node_{expr_node} {}
    virtual ~AssignmentNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    const TreeNode* get_decl() const { return decl_node_; }
    const TreeNode* get_expr() const { return expr_node_; }
};

class ValueNode final : public TreeNode {
  private:
    int value_;
  public:
    ValueNode(int value)
        : TreeNode{TreeNodeType::kValueNode}, value_{value} {}
    virtual ~ValueNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    int get_value() const { return value_; }
};

class PrintNode final : public TreeNode {
  private:
    TreeNode* node_to_print_;
  public:
    PrintNode(TreeNode* node_to_print = nullptr)
        : TreeNode{TreeNodeType::kPrintNode}, node_to_print_{node_to_print} {}
    virtual ~PrintNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    const TreeNode* get_to_print() const { return node_to_print_; }
};

enum class BinaryOpType {
    kAdd     = 0,
    kSub     = 1,
    kMul     = 2,
    kDiv     = 3,
    kModulus = 4,
};

class BinOpNode final : public TreeNode {
  private:
    BinaryOpType bin_op_;
    TreeNode* left_operand_;
    TreeNode* right_operand_;
  public:
    BinOpNode(BinaryOpType bin_op, TreeNode* left_operand = nullptr, TreeNode* right_operand = nullptr)
        : TreeNode{TreeNodeType::kBinOpNode}, bin_op_{bin_op}, left_operand_{left_operand}, right_operand_{right_operand} {}
    virtual ~BinOpNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    BinaryOpType get_bin_op() const { return bin_op_; }
    const TreeNode* get_left_op() const { return left_operand_; }
    const TreeNode* get_right_op() const { return right_operand_; }
};

enum class UnaryOpType {
    kNot   = 0,
    kMinus = 1,
    kPlus  = 2,
};

class UnOpNode final : public TreeNode {
  private:
    UnaryOpType un_op_;
    TreeNode* operand_;
  public:
    UnOpNode(UnaryOpType un_op, TreeNode* operand = nullptr)
        : TreeNode{TreeNodeType::kUnOpNode}, un_op_{un_op}, operand_{operand} {}
    virtual ~UnOpNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    UnaryOpType get_unary_op() const { return un_op_; }
    const TreeNode* get_operand() const { return operand_; }
};

enum class LogicalOpType {
    kEqual          = 0,
    kNotEqual       = 1,
    kBelow          = 2,
    kEqualOrBelow   = 3,
    kGreater        = 4,
    kEqualOrGreater = 5,
    kAnd            = 6,
    kOr             = 7,
    kXor            = 8,
};

class LogOpNode final : public TreeNode {
  private:
    LogicalOpType log_op_;
    TreeNode* left_operand_;
    TreeNode* right_operand_;
  public:
    LogOpNode(LogicalOpType log_op, TreeNode* left_operand = nullptr, TreeNode* right_operand = nullptr)
        : TreeNode{TreeNodeType::kBinOpNode}, log_op_{log_op}, left_operand_{left_operand}, right_operand_{right_operand} {}
    virtual ~LogOpNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    LogicalOpType get_logical_op() const { return log_op_; }
    const TreeNode* get_left_op() const { return left_operand_; }
    const TreeNode* get_right_op() const { return right_operand_; }
};

class QuestionMarkNode final : public TreeNode {
  private:

  public:
    QuestionMarkNode()
        : TreeNode{TreeNodeType::kQuestionMarkNode} {}
    virtual ~QuestionMarkNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;
};

class ScopeNode final : public TreeNode {
  private:
    ScopeNode* parent_node_;
    std::deque<TreeNode*> nodes_;

  public:
    ScopeNode(ScopeNode* parent_node)
        : TreeNode{TreeNodeType::kScopeNode}, parent_node_{parent_node} {};
      virtual ~ScopeNode() override = default;


    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    const TreeNode* get_parent_scope() const { return parent_node_; } 
    const std::deque<TreeNode*>* get_nodes() const { return &nodes_; }

    ScopeNode* add_node(TreeNode* new_node) {
        nodes_.push_front(new_node);
        return this; 
    }

    ScopeNode* close_scope() {
        return parent_node_;
    }
};

class ExprNode final : public TreeNode {
  private:
    TreeNode* expr_node_;
  public:
    ExprNode(TreeNode* expr_node) 
        : TreeNode{TreeNodeType::kExprNode}, expr_node_{expr_node} {}
    virtual ~ExprNode() override = default;

    void accept(NodeVisitor* visitor) const override { visitor->visit(*this); }
    void print(std::ofstream& file) const override;

    const TreeNode* get_expr() const { return expr_node_; }
};

class Ast final {
  public:
    Ast() = default;
    TreeNode* root_= nullptr;
    std::vector<TreeNode*> nodes_{};
    Ast(const Ast&) = delete;
    Ast& operator=(const Ast&) = delete;

    Ast(Ast&& other) noexcept 
        : root_(other.root_), nodes_(std::move(other.nodes_)) {
        other.root_ = nullptr;
    }

    Ast& operator=(Ast&& other) noexcept {
        if (this != &other) { 
            root_ = other.root_;
            nodes_ = std::move(other.nodes_);
            other.root_ = nullptr;
        }
        return *this;
    }

    ~Ast() {
      for (auto it : nodes_) {
        delete it;
      }

  }
  public:
    bool empty() const {
        return nodes_.empty();
    }

    void print() const;

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

    VarDerefNode* insert_var_deref_node(const std::string& variable_name) {
        VarDerefNode* new_var_deref_node = new VarDerefNode{variable_name};
        nodes_.push_back(new_var_deref_node);
        return static_cast<VarDerefNode*>(nodes_.back());
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

    UnOpNode* insert_un_op_node(UnaryOpType bin_op, TreeNode* operand = nullptr) {
        UnOpNode* new_bin_op_node = new UnOpNode{bin_op, operand};
        nodes_.push_back(new_bin_op_node);
        return static_cast<UnOpNode*>(nodes_.back());
    }
 

    LogOpNode* insert_log_op_node(LogicalOpType log_op, TreeNode* left_operand = nullptr, TreeNode* right_operand = nullptr) {
        LogOpNode* new_log_op_node = new LogOpNode{log_op, left_operand, right_operand};
        nodes_.push_back(new_log_op_node);
        return static_cast<LogOpNode*>(nodes_.back());
    }

    QuestionMarkNode* insert_question_mark_node() {
        QuestionMarkNode* new_question_mark_node = new QuestionMarkNode();
        nodes_.push_back(new_question_mark_node);
        return static_cast<QuestionMarkNode*>(nodes_.back());
    }

    ScopeNode* insert_scope_node(ScopeNode* parent_node = nullptr) {
        ScopeNode* new_scope_node = new ScopeNode{parent_node};
        nodes_.push_back(new_scope_node);
        return static_cast<ScopeNode*>(nodes_.back());
    }

    ExprNode* insert_expr_node(TreeNode* expr_node = nullptr) {
        ExprNode* new_expr_node = new ExprNode{expr_node};
        nodes_.push_back(new_expr_node);
        return static_cast<ExprNode*>(nodes_.back());
    }
};

} // namespace AST

#endif // AST_HPP
