/* Check Balanced
Implement a function to check if a binary tree is balanced. For the purposes of
this question, a balanced tree is defined to be a tree such that the heights of
the two subtrees of any node never differs by more than one. */

#include <iostream>

namespace {

struct Node {
  Node *left;
  Node *right;
};

int MeasureDepth(Node *n) {
  int depth_left, depth_right;
  if (n->left == nullptr)
    depth_left = 0;
  if ((depth_left = MeasureDepth(n->left)) == -1)
    return -1;
  if (n->right == nullptr)
    depth_right = 0;
  if ((depth_right = MeasureDepth(n->right)) == -1)
    return -1;

  if (std::abs(depth_left-depth_right) > 1)
    return -1;
  return std::max(depth_left, depth_right) + 1;
}

bool CheckBalanced(Node* root) {
  return MeasureDepth(root) == -1;
}

} // namespacee

int main(void) {
  return 0;
}

