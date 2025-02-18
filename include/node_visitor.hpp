#ifndef NODE_VISITOR_HPP
#define NODE_VISITOR_HPP

// should not include ast.hpp

namespace ast {

// forward decl
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
class QuestionMarkNode;
class ScopeNode;
class ExprNode;

class NodeVisitor {
  public: 
    virtual ~NodeVisitor() = default;

    virtual void visit_all() = 0; 
    virtual void visit(const WhileNode& node) = 0;
    virtual void visit(const IfNode& node) = 0;
    virtual void visit(const ElseNode& node) = 0;
    virtual void visit(const DeclNode& node) = 0;
    virtual void visit(const VarDerefNode& node) = 0;
    virtual void visit(const AssignmentNode& node) = 0;
    virtual void visit(const ValueNode& node) = 0;
    virtual void visit(const PrintNode& node) = 0;
    virtual void visit(const BinOpNode& node) = 0;
    virtual void visit(const UnOpNode& node)  = 0;
    virtual void visit(const LogOpNode& node) = 0;
    virtual void visit(const QuestionMarkNode& node) = 0;
    virtual void visit(const ScopeNode& node) = 0;
    virtual void visit(const ExprNode& node) = 0;
};

} // namespace

#endif // NODE_VISITOR_HPP
