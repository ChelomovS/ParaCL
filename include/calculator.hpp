#ifndef CACLULATOR_HPP
#define CACLULATOR_HPP

#include "ast.hpp"
#include "driver.hpp"
#include "entity_table.hpp"

#include <FlexLexer.h>

class calculator final {

// NOTE ScopeNode 
// int calculate() override {
//   for (auto it : nodes_)
//     if (it) {
//       it->calculate();
//     }
//   return 0;
// }


// NOTE log op 
// int calculate() override {
//   int left_value  = left_operand_->calculate();
//   int right_value = right_operand_->calculate();

//   switch (log_op_) {
//     case LogicalOpType::kEqual: 
//       return (left_value == right_value);
//     case LogicalOpType::kNotEqual:
//       return (left_value != right_value);
//     case LogicalOpType::kBelow:
//       return (left_value < right_value);
//     case LogicalOpType::kEqualOrBelow: 
//       return (left_value <= right_value);
//     case LogicalOpType::kGreater:
//       return (left_value > right_value);
//     case LogicalOpType::kEqualOrGreater:
//       return (left_value >= right_value);
//     defaulf:
//       // мб тут че нить
//       break;
//   } 
// }


// NOTE - while
// int calculate() override {
//   bool condition = static_cast<bool>(condition_node_->calculate());
//   while (condition) {
//     scope_node_->calculate();
//     condition = static_cast<bool>(condition_node_->calculate());
//   }
//   return 0;
// }

// NOTE - IfNode
// int calculate() override {
//   if (condition_node_->calculate())
//     scope_node_->calculate();
//   else 
//     else_node_->calculate();
//   return 0;
// }

// NOTE - ScopeNode
// int calculate() override {
//   scope_node_->calculate();
//   return 0;
// }

// NOTE DeclNode

// NOTE Value node
// int calculate() override {
//   return value_;
// }

// NOTE - PRINT node
// int calculate() override {
//   int value = node_to_print_->calculate();
//   std::cout << value;
//   return std::cout.good();
// }

// NOTE BinOP
// int calculate() override {
    //   int left_value  = left_operand_->calculate();
    //   int right_value = right_operand_->calculate();

    //   switch (bin_op_) {
    //     case BinaryOpType::kAdd: 
    //       return left_value + right_value;
    //     case BinaryOpType::kSub:
    //       return left_value - right_value;
    //     case BinaryOpType::kMul:
    //       return left_value * right_value;
    //     case BinaryOpType::kDiv:
    //       return left_value / right_value;
    //     defaulf:
    //       // мб тут че нить
    //       break;
    //     return 0;
    //   } 
    // }
};

#endif // CACLULATOR_HPP
