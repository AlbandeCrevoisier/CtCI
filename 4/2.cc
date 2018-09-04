/* Minimal Tree
Given a sorted array with unique integers elements, write an algorithm to create
a binary search tree with minimal height. */

#include <iostream>
#include <vector>

namespace {

struct Node {
  int val;
  Node *left;
  Node *right;
};

int MinimalTree(std::vector<Node> &nodes, int start, int end) {
  if (start == end) {
    return start;
  } else if (end - start == 1) {
    nodes[end].left = &nodes[start];
    return end;
  }
  int mid = start + ((end-start+1) % 2 == 0) ? (end-start+1)/2 : (end-start)/2;
  int left = MinimalTree(nodes, start, mid-1);
  int right = MinimalTree(nodes, mid+1, end);
  nodes[mid].left = &nodes[left];
  nodes[mid].right = &nodes[right];
  return mid;
}

} // namespace

int main(void) {
  std::vector<int> numbers = {1, 2, 3, 5, 7, 9};
  std::vector<Node> nodes;
  for (auto i : numbers)
    nodes.push_back({i, nullptr, nullptr});
  int root = MinimalTree(nodes, 0, 5);
  return 0;
}

