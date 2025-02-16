#include "interpreter.hpp"

#include <ostream>
#include <stdexcept>
#include <vector>

#include <spdlog/spdlog.h>

#include "ast.hpp"

namespace intpr {

#if 1
#define trace_calls() spdlog::debug("{}", __PRETTY_FUNCTION__)
#else 
#define trace_calls() ;
#endif // trace_calls

void Interpreter::visit_all() {
    trace_calls();

    ast_.root_->accept(this);
}

void Interpreter::visit(const ast::WhileNode& node) {
    trace_calls();

    const ast::TreeNode* cond_node = node.get_cond(); 
    const ast::TreeNode* scope_node = node.get_scope();

    EntityTable::EntityScope scope = EntityTable::EntityScope{};

    while (true) {
        cond_node->accept(this);
    
        int expr_value = eval_stack.back();
        eval_stack.pop_back();
        if (!expr_value) { break; }

        entity_table.push_scope(scope);
        scope_node->accept(this);
        scope = entity_table.top_scope();
        entity_table.pop_scope();
    }
}

void Interpreter::visit(const ast::IfNode& node) {
    trace_calls();

    const ast::TreeNode* cond_node = node.get_cond();
    const ast::TreeNode* scope_node = node.get_scope();
    const ast::TreeNode* else_node = node.get_else();

    cond_node->accept(this);
    int cond_value = eval_stack.back();
    eval_stack.pop_back();
        
    entity_table.push_scope();

    if (cond_value) {
        scope_node->accept(this);
    } else if (else_node != nullptr) {
        else_node->accept(this);
    }

    entity_table.pop_scope();
}

void Interpreter::visit(const ast::ElseNode& node) {
    trace_calls();

    const ast::TreeNode* scope_node = node.get_scope();

    scope_node->accept(this);
}

void Interpreter::visit(const ast::DeclNode& node) {
    trace_calls();
    
    (void)node;

    // do nothing 
}

void Interpreter::visit(const ast::VarDerefNode& node) {
    trace_calls();

    std::string var_name = node.get_name();

    if (entity_table.is_declared_global(var_name)) {
        int var_value = entity_table.lookup(var_name);
        eval_stack.push_back(var_value);
    } else {
        spdlog::error("undecalred variable");
        throw std::runtime_error("undeclared variable");
    }
}

void Interpreter::visit(const ast::AssignmentNode& node) {
    trace_calls();

    const ast::DeclNode* decl_node = static_cast<const ast::DeclNode*>(node.get_decl());
    const ast::TreeNode* expr_node = node.get_expr();

    expr_node->accept(this);
    int expr_value = eval_stack.back();
    // dont pop because c = b = a = expr; chain assignment

    std::string var_name = decl_node->get_name();

    bool is_declared_scope = entity_table.is_declared_global(var_name);
    spdlog::trace("is variable [{}] decalred in scope [{}]", var_name, is_declared_scope);
    entity_table.log();
    if (is_declared_scope) {
        entity_table.assign(var_name, expr_value);
    } else {
        entity_table.declare(var_name, expr_value);
    }
    entity_table.log();
}

void Interpreter::visit(const ast::ValueNode& node) {
    trace_calls();

    eval_stack.push_back(node.get_value());
}

void Interpreter::visit(const ast::PrintNode& node) {
    trace_calls();

    const ast::TreeNode* expr_node = node.get_to_print();
    expr_node->accept(this);

    int value_to_print = eval_stack.back();
    eval_stack.pop_back();

#if 0
    std::string helper = "Print: ";
#else
    std::string helper = "";
#endif // helper

    std::cout << helper << value_to_print << std::endl;
}

void Interpreter::visit(const ast::BinOpNode& node) {
    trace_calls();
 
    const ast::TreeNode* left_node = node.get_left_op();
    const ast::TreeNode* right_node = node.get_right_op();
    ast::BinaryOpType bin_op = node.get_bin_op();

    left_node->accept(this);
    int left_op = eval_stack.back();    
    eval_stack.pop_back();

    right_node->accept(this);
    int right_op = eval_stack.back();
    eval_stack.pop_back();

    int res = 0;
    switch (bin_op) {
        using enum ast::BinaryOpType;
        case kAdd:
            res = left_op + right_op;
            break;
        case kSub:
            res = left_op - right_op;
            break;
        case kMul:
            res = left_op * right_op;
            break;
        case kDiv:
            if (right_op == 0) {
                throw std::runtime_error("division by 0");
            }
            
            res = left_op / right_op;
            break;
        default:
            spdlog::critical("unknown binary operation");
            assert(0 && "unknown binary operation");
    }

    eval_stack.push_back(res);
}

void Interpreter::visit(const ast::LogOpNode& node) {
    trace_calls();
 
    const ast::TreeNode* left_node = node.get_left_op();
    const ast::TreeNode* right_node = node.get_right_op();
    ast::LogicalOpType logical_op = node.get_logical_op();

    left_node->accept(this);
    int left_op = eval_stack.back();    
    eval_stack.pop_back();

    right_node->accept(this);
    int right_op = eval_stack.back();
    eval_stack.pop_back();

    spdlog::trace("left value: {}, right value: {}", left_op, right_op);
    int res = 0;
    switch (logical_op) {
        using enum ast::LogicalOpType;
        case kEqual:
            res = (left_op == right_op);
            spdlog::trace("kEqual res = {}", res);
            break;
        case kNotEqual:
            res = (left_op != right_op);
            spdlog::trace("kNotEqual res = {}", res);
            break;
        case kBelow:
            res = (left_op < right_op);
            spdlog::trace("kBelow res = {}", res);
            break;
        case kEqualOrBelow:
            res = (left_op <= right_op);
            spdlog::trace("kEqualOrBelow res = {}", res);
            break;
        case kGreater:
            res = (left_op > right_op);
            spdlog::trace("kGreater res = {}", res);
            break;
        case kEqualOrGreater:
            res = (left_op >= right_op);
            spdlog::trace("kEqualOrGreater res = {}", res);
            break;
        default:
            spdlog::critical("unknown logical operation");
            assert(0 && "unknown logical operation");
    }

    eval_stack.push_back(res);   
}

void Interpreter::visit(const ast::QuestionMarkNode& node) {
    trace_calls();
    
    (void)node;

    int input_value = 0;
#if 0
    std::string helper = "Input: ";
#else 
    std::string helper = "";
#endif // helper

    std::cout << helper;
    std::cin >> input_value;
    eval_stack.push_back(input_value);
}   

void Interpreter::visit(const ast::ScopeNode& node) {
    trace_calls();

    // entity_table.push_scope(); // push new scope on top

    auto nodes = node.get_nodes();
    for (auto it: *nodes) {
        if (!it) {
            continue;
        }

        it->accept(this);
    }

    // entity_table.pop_scope(); // pop scope from top
}

void Interpreter::visit(const ast::ExprNode& node) {
    trace_calls();

    const ast::TreeNode* expr_node = node.get_expr();
    expr_node->accept(this);
}

} // namespace intpr
