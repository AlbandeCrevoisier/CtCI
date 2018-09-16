/* Successor
Write an algorithm to find the "next" node (i.e., in-order successor) of a given
node in a binary search tree. You may assume that each node has a link to its
parent. */

#include <iostream>

namespace {

struct Node {
  int val;
  Node *parent;
  Node *left;
  Node *right;
};

Node* Successor(Node *n) {
  if (n == nullptr)
    return n;
  /* No right son: go up until you go right. */
  if (n->right == nullptr) {
    Node *walker = n->parent;
    while (walker != nullptr && walker == walker->parent->right)
      walker = walker->parent;
    if (walker == nullptr)
      return nullptr;
    else
      return walker->parent;
  }
  /* Has a right son: go down to the bottom left of the right subtree. */
  Node *walker = n->right;
  while (walker->left != nullptr)
    walker = walker->left;
  return walker;
}

} // namespace

int main(void) {
  Node n1 = {9, nullptr, nullptr, nullptr};
  Node n2 = {5, &n1, nullptr, nullptr};
  n1.left = &n2;
  Node n3 = {17, &n1, nullptr, nullptr};
  n1.right = &n3;
  Node n4 = {3, &n2,nullptr, nullptr};
  n2.right = &n4;
  Node n5 = {13, &n3,nullptr, nullptr};
  n3.left = &n5;
  std::cout << Successor(&n4)->val << std::endl;
  std::cout << Successor(&n1)->val << std::endl;
  return 0;
}

