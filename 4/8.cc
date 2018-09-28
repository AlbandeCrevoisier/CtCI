/* First Common Ancestor
Find the first common ancestor of two nodes in a binary tree. Avoid storing
additional nodes.
Hypothesis: each node has a reference to its parent node. */

#include <iostream>

namespace {

struct Node {
  Node *parent;
  Node *left;
  Node *right;
};

*Node FirstCommonAncestor(Node *tree, Node *first_node, Node *second_node) {
  int depth_first_node = 0;
  int depth_second_node = 0;
  Node *tmp = first_node;
  while (tmp != tree) {
    tmp = tmp.parent;
    depth_first_node++;
  }
  tmp = second_node;
  while (tmp != tree) {
    tmp = tmp.parent;
    depth_second_node++;
  }
  int min_depth = std::min(depth_first_node, depth_second_node) - 1;
  Node *first_ancestor = first_node;
  Node *second_ancestor = second_node;
  for (i = 0; i < depth_first_node - min_depth; i++)
    first_ancestor = first_ancestor.parent;
  for (i = 0; i < depth_second_node - min_depth; i++)
    second_ancestor = second_ancestor.parent;
  while (first_ancestor != second_ancestor) {
    first_ancestor = first_ancestor.parent;
    second_ancestor = second_ancestor.parent;
  }
  return first_ancestor;
}

} // namespace

int main(void) {
  Node n0, n1, n2, n3, n4, n5, n6, n7, n8;
  n8 = {&n6, nullptr, nullptr};
  n7 = {&n6, nullptr, nullptr};
  n6 = {&n4, &n7, &n8};
  n5 = {&n3, nullptr, nullptr};
  n4 = {&n2, &n6, nullptr};
  n3 = {&n2, &n5, nullptr};
  n2 = {&n0, &n3, &n4};
  n1 = {&n0, nullptr, nullptr};
  n0 = {nullptr, &n1, &n2};
  std::cout << FirstCommonAncestor(&n0, &n5, &n7) << ":" << &n2 << std::endl;
  return 0;
}

