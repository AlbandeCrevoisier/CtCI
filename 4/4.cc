/* Check Balanced
Implement a function to check if a binary tree is balanced. For the purposes of
this question, a balanced tree is defined to be a tree such that the heights of
the two subtrees of any node never differs by more than one. */

#include <iostream>
#include <vector>

namespace {

struct Node {
  Node *left;
  Node *right;
};

int MeasureDepth(Node *n) {
  int depth_left, depth_right;
  if (n->left == nullptr)
    depth_left = 0;
  else if ((depth_left = MeasureDepth(n->left)) == -1)
    return -1;
  if (n->right == nullptr)
    depth_right = 0;
  else if ((depth_right = MeasureDepth(n->right)) == -1)
    return -1;

  if (std::abs(depth_left-depth_right) > 1)
    return -1;
  return std::max(depth_left, depth_right) + 1;
}

bool CheckBalanced(Node* root) {
  return MeasureDepth(root) != -1;
}

} // namespacee

int main(void) {
  std::vector<Node> m = {{nullptr, nullptr},
                         {nullptr, nullptr},
                         {nullptr, nullptr},
                         {nullptr, nullptr},
                         {nullptr, nullptr}};
  std::vector<Node> n = {{nullptr, nullptr},
                         {nullptr, nullptr},
                         {nullptr, nullptr}};
  m[0].left = &m[1];
  m[0].right = &m[2];
  m[2].left = &m[3];
  m[2].right = &m[4];
  n[0].left = &n[1];
  n[1].left = &n[2];
  std::cout << "m" << CheckBalanced(&m[0]) << std::endl;
  std::cout << "n" << CheckBalanced(&n[0]) << std::endl;
  return 0;
}

