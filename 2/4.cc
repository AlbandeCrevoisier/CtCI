/* Partition
Write code to partition a linked list around a value x, such that all nodes
less than x come before all nodes greater than or equal to x. If x is
contained within the list, the values of x only need to be after the
elements less than x. The partition element x can appear anywhere in the
"right partition"; it does not need to appear between the left and right
partitions. */

#include <iostream>
#include <list>

namespace {

/* Not using std::list */
struct Node {
  int data;
  Node *next_node;
};

struct LinkedList {
  Node *head;
};

int PushFront(LinkedList &l, Node *n) {
  if (n->next_node != nullptr)
    return -1;
  if (l.head == nullptr) {
    l.head = n;
  } else {
    n->next_node = l.head;
    l.head = n;
  }
  return 0;
}

LinkedList PartitionLinkedList(LinkedList l, int threshold) {
  LinkedList low = {nullptr}, high = {nullptr};
  Node *low_end = nullptr;
  Node *n = l.head;
  while (n != nullptr) {
    Node *p = n->next_node;
    n->next_node = nullptr;
    if (n->data < threshold) {
      if (low.head == nullptr)
        low_end = n;
      PushFront(low, n);
    } else {
      PushFront(high, n);
    }
    n = p;
  }
  low_end->next_node = high.head;
  return low;
}

/* Using std::list */
std::list<int> PartitionList(std::list<int> l, int threshold) {
  std::list<int> high, low;
  for (auto e : l) {
    if (e < threshold)
      low.emplace_back(e);
    else
      high.emplace_back(e);
  }
  high.insert(high.begin(), low.begin(), low.end());
  return high;  
}

} // namespace

int main(void) {
  int threshold = 5;
  std::cout << "LinkedList" << std::endl;
  LinkedList ll = {nullptr};
  Node n0 = {1, nullptr};
  Node n1 = {2, nullptr};
  Node n2 = {7, nullptr};
  Node n3 = {6, nullptr};
  Node n4 = {0, nullptr};
  Node n5 = {10, nullptr};
  Node n6 = {42, nullptr};
  PushFront(ll, &n0);
  PushFront(ll, &n1);
  PushFront(ll, &n2);
  PushFront(ll, &n3);
  PushFront(ll, &n4);
  PushFront(ll, &n5);
  PushFront(ll, &n6);
  ll = PartitionLinkedList(ll, threshold);
  for (Node *n = ll.head; n != nullptr; n = n->next_node)
    std::cout << n->data << ":";
  std::cout << std::endl;
  std::cout << "std::list" << std::endl;
  std::list<int> l = {1,2,7,6,0,10,42};
  l = PartitionList(l, threshold);
  for (auto e : l)
    std::cout << e << ":";
  std::cout << std::endl;
  return 0;
}
