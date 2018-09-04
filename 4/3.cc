/* List of Depths
Given a binary tree, design an algorithm which creates a linked list of all the
nodes at each depth. */

#include <iostream>
#include <list>
#include <vector>

namespace {

struct Node {
  Node *left;
  Node *right;
};

void ListDepth(Node *n, std::vector<std::list<Node*>> &depths, int depth) {
  if (n == nullptr)
    return;
  while (depths.size() <= uint(depth))
    depths.push_back({});
  depths[depth].push_back(n);
  ListDepth(n->left, depths, depth+1);
  ListDepth(n->right, depths, depth+1);
}

} // namespace

int main(void) {
  return 0;
}

