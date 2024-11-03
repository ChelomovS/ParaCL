#ifndef TREE_HPP_
#define TREE_HPP_

namespace Ast {

enum class TreeNodeType {
    kWhileNode = 0,

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

class Tree final {
    TreeNode* root;
};

class WhileNode : public TreeNode {
  private:
    TreeNode* condition_node_;
    TreeNode* scope_node_;
  public:
    WhileNode(TreeNode* parent, TreeNode* condition_node, TreeNode* scope_node) 
        : TreeNode{parent, TreeNodeType::kWhileNode}, condition_node_(condition_node), scope_node_(scope_node) {}
    // FIXME бля че писать 
}; 

} // namespace Ast

#endif // TREE_HPP_
