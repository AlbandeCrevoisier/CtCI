/* BST Sequence
A binary search tree was created by traversing an array left to right and
inserting the elements. Given a BST with distincts elements, print all the
possible arrays that could have led to this tree.
Hypothesis: left <= root < right */

#include <iostream>
#include <list>

namespace {

struct Node {
  int val;
  Node *left, *right;
};

void ArrayFromBST(Node *tree);
void RecurseBST(Node *n, std::list<int> prefix, std::list<Node*> next_nodes);

void ArrayFromBST(Node *tree) {
  std::list<Node*> next_nodes;
  next_nodes.push_back(tree->left);
  next_nodes.push_back(tree->right);
  RecurseBST(tree, std::list<int> {}, next_nodes);
}

void RecurseBST(Node *n, std::list<int> prefix, std::list<Node*> next_nodes) {
  prefix.push_back(n->val);
  next_nodes.remove(n);
  if (n->left != nullptr)
    next_nodes.push_back(n->left);
  if (n->right != nullptr)
    next_nodes.push_back(n->right);
  for (auto nn : next_nodes)
    RecurseBST(nn, prefix, next_nodes);
  if (next_nodes.empty()) {
    for (auto e : prefix)
      std::cout << e << ", ";
    std::cout << std::endl;
  }
}

} // namespace

int main(void) {
  return 0;
}

