#include "ast.hpp"

#include <cassert>

#if 0
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
#endif

namespace Ast { 
  int execute_node(TreeNode* node) {
    auto node_type = node.get_type();
    switch (node_type) {
        case kWhileNode:

        break;

        case kIfNode:

        break;

        case kElseNode:

        break;

        case kDeclNode: 

        break;

        case kAssignmentNode: 

        break;

        case kValueNode:

        break;

        case kBinOpNode:

        break;

        case kQuestionMarkNode:

        break;

        case kScopeNode:

        break;

        default: assert(0);
    }
  }
}