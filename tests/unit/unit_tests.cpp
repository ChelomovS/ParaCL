#include <stdexcept> // for std::runtime_error
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <gtest/gtest.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/common.h>

#include "interpreter/interpreter.hpp"
#include "parser/ast.hpp"

class InterpreterTest : public ::testing::Test {
protected:
    void SetUp() override {
        spdlog::trace("start test");
        buffer_ = new std::stringstream();
        cout_buffer_ = std::cout.rdbuf(buffer_->rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(cout_buffer_);
        delete buffer_;
    }

    void RunAndCheckOutput(const std::string& expected_output) {
        intpr::Interpreter interpreter(&ast_);
        interpreter.visit_all();
        EXPECT_EQ(buffer_->str(), expected_output);
    }

    template <typename ExpectedException>
    void RunAndCheckForThrow() {
        intpr::Interpreter interpreter(&ast_);
        EXPECT_THROW({interpreter.visit_all();}, ExpectedException);
    }

    ast::Ast ast_;
    std::stringstream* buffer_;
    std::streambuf* cout_buffer_;
};

TEST_F(InterpreterTest, SimplePrint) {
    ast_.root_ = ast_.insert_scope_node()->add_node(
        ast_.insert_expr_node(
            ast_.insert_print_node(
                ast_.insert_expr_node(
                    ast_.insert_value_node(42)
                )
            )
        )
    );

    std::string expected_output = "42\n";
    RunAndCheckOutput(expected_output);
}

TEST_F(InterpreterTest, SimpleArithmetic) {
    ast_.root_ = ast_.insert_scope_node()->add_node(
        ast_.insert_expr_node(
            ast_.insert_print_node(
                ast_.insert_expr_node(
                    ast_.insert_bin_op_node(
                        ast::BinaryOpType::kMul,
                        ast_.insert_expr_node(ast_.insert_value_node(7)),
                        ast_.insert_expr_node(ast_.insert_value_node(6))
                    )
                )
            )
        )
    );

    std::string expected_output = "42\n";
    RunAndCheckOutput(expected_output);
}

TEST_F(InterpreterTest, SimpleAssignment) {
    ast_.root_ = ast_.insert_scope_node()->add_node(
        ast_.insert_expr_node(
            ast_.insert_print_node(
                ast_.insert_assignment_node(
                    ast_.insert_decl_node("x"),
                    ast_.insert_expr_node(
                        ast_.insert_value_node(42)
                    )
                )
            )
        )
    );

    std::string expected_output = "42\n";
    RunAndCheckOutput(expected_output);
}

TEST_F(InterpreterTest, SimpleIf) {
    ast_.root_ = ast_.insert_scope_node()->add_node(
        ast_.insert_if_node(
            ast_.insert_expr_node(
                ast_.insert_log_op_node(
                    ast::LogicalOpType::kNotEqual,
                    ast_.insert_expr_node(ast_.insert_value_node(1)),
                    ast_.insert_expr_node(ast_.insert_value_node(2))
                )
            ),
            ast_.insert_expr_node(ast_.insert_print_node(ast_.insert_expr_node(ast_.insert_value_node(42)))),
            nullptr // no else
        )
    );

    std::string expected_output = "42\n";
    RunAndCheckOutput(expected_output);
}

TEST_F(InterpreterTest, ScopeTest) {
    ast::ScopeNode scope = ast_.insert_scope_node();
    
    scope.add_node(
        ast_.insert_print_node(
            ast_.insert_expr_node(
                ast_.insert_var_deref_node("x")
            )
        )
    );
    scope.add_node(
        ast_.insert_if_node(
            ast_.insert_expr_node(
                ast_.insert_value_node(1)                
            ),
            ast_.insert_expr_node(
                ast_.insert_assignment_node(
                    ast_.insert_decl_node("x"),
                    ast_.insert_expr_node(
                        ast_.insert_value_node(1)
                    )
                )
            ),
            nullptr // no else
        )
    );
    
    ast_.root_ = &scope;
    RunAndCheckForThrow<std::runtime_error>();
}

int main(int argc, char** argv) {
    auto logger = spdlog::basic_logger_mt("paracl", "paracl_test.log", /*override log?*/ true);
    spdlog::set_default_logger(logger);

#if defined (NDEBUG)
    spdlog::set_level(spdlog::level::info);
#else // NDEBUG
    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_level(spdlog::level::trace);
#endif // NDEBUG

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
