/* Delete Middle Node
Implement an algorithm to delete a node in the middle (i.e., any node but
the first and last node, not necessarily the exact middle) of a singly
linked list, given only access to that node.
*/

#include <iostream>

namespace {

struct Node {
  int data;
  Node *next;
};

int DeleteMiddleNode(Node *n) {
  if (n == nullptr)
    return -1;
  if (n->next == nullptr)
    return -2;

  n->data = (n->next)->data;
  n->next = (n->next)->next;
  return 0;
}

} // namespace

int main(void) {
  Node list = {1, nullptr};
  Node mid = {2, nullptr};
  Node end = {3, nullptr};
  list.next = &mid;
  (list.next)->next = &end;
  if (int err = DeleteMiddleNode(list.next) < 0)
    std::cout << "Error: " << err << std::endl;
  else
    std::cout << (list.next)->data << std::endl;
  return 0;
}

