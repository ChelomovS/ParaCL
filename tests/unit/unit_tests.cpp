#include <gtest/gtest.h>

#include "interpreter.hpp"
#include "ast.hpp"


TEST(Interpreter, simple_print) {
    ast::Ast ast{};
    ast.root_ = ast.insert_scope_node()->add_node(ast.insert_expr_node(ast.insert_print_node
                                                 (ast.insert_expr_node(ast.insert_value_node(42)))));
    intpr::Interpreter interpreter(ast);
    std::stringstream buffer;
    std::streambuf* сout_buffer = std::cout.rdbuf(buffer.rdbuf());
    interpreter.visit_all();
    std::cout.rdbuf(сout_buffer);
    std::string expected_output = "Print: 42\n";
    EXPECT_EQ(buffer.str(), expected_output);
}

TEST(Interpreter, simple_arithmetic) {
    ast::Ast ast{};
    ast.root_ = ast.insert_scope_node()->add_node(ast.insert_expr_node(ast.insert_print_node(ast.insert_expr_node
                                                 (ast.insert_bin_op_node(ast::BinaryOpType::kMul, 
                                                  ast.insert_expr_node(ast.insert_value_node(7)),
                                                  ast.insert_expr_node(ast.insert_value_node(6)))))));
    intpr::Interpreter interpreter(ast);
    std::stringstream buffer;
    std::streambuf* сout_buffer = std::cout.rdbuf(buffer.rdbuf());
    interpreter.visit_all();
    std::cout.rdbuf(сout_buffer);
    std::string expected_output = "Print: 42\n";
    EXPECT_EQ(buffer.str(), expected_output);
}

TEST(Interpreter, simple_assignment) {
    ast::Ast ast{};

    ast.root_ = ast.insert_scope_node()->add_node(ast.insert_expr_node(ast.insert_print_node
                                                 (ast.insert_assignment_node(ast.insert_decl_node("x"),
                                                  ast.insert_expr_node(ast.insert_value_node(42))))));
                                         
    intpr::Interpreter interpreter(ast);
    std::stringstream buffer;
    std::streambuf* сout_buffer = std::cout.rdbuf(buffer.rdbuf());
    interpreter.visit_all();
    std::cout.rdbuf(сout_buffer);
    std::string expected_output = "Print: 42\n";
    EXPECT_EQ(buffer.str(), expected_output);
}

TEST(Interpreter, simple_if) {
    ast::Ast ast{};

    ast.root_ = ast.insert_scope_node()->add_node(ast.insert_if_node(ast.insert_expr_node(ast.insert_log_op_node
                                                 (ast::LogicalOpType::kNotEqual, 
                                                 (ast.insert_expr_node(ast.insert_value_node(1)), 
                                                  ast.insert_expr_node(ast.insert_value_node(2))), 
                                                  ast.insert_expr_node(ast.insert_print_node
                                                 (ast.insert_expr_node(ast.insert_value_node(42))))))));
                                                
    intpr::Interpreter interpreter(ast);
    std::stringstream buffer;
    std::streambuf* сout_buffer = std::cout.rdbuf(buffer.rdbuf());
    interpreter.visit_all();
    std::cout.rdbuf(сout_buffer);
    std::string expected_output = "Print: 42\n";
    EXPECT_EQ(buffer.str(), expected_output);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}