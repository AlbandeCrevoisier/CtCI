/* Validate BST
Implement a function to check if a binary tree is a binary search tree. */

#include <iostream>
#include <climits>

namespace {

struct Node {
  int val;
  Node *left;
  Node *right;
};

bool ValidateBST(Node *n, int &previous_value) {
  if (n == nullptr)
    return true;
  if (n->left != nullptr && n->left->val > n->val)
    return false;
  if (n->right != nullptr && n->right->val <= n->val)
    return false;

  if (n->left != nullptr && !ValidateBST(n->left, previous_value))
    return false;
  if (previous_value > n->val)
    return false;
  previous_value = n->val;
  if (n->right != nullptr && !ValidateBST(n->right, previous_value))
    return false;

  return true;
}

} // namespace

int main(void) {
  int prev = INT_MIN;
  Node n1 = {42, nullptr, nullptr};
  Node n2 = {9, nullptr, nullptr};
  Node n3 = {17, nullptr, nullptr};
  Node n4 = {2, nullptr, nullptr};
  n1.left = &n2;
  n1.right = &n3;
  std::cout << ValidateBST(&n1, prev) << std::endl;
  n1.right = nullptr;
  n2.left = &n4;
  n2.right = &n3;
  prev = INT_MIN;
  std::cout << ValidateBST(&n1, prev) << std::endl;
  return 0;
}

