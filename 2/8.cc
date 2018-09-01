/* Loop Detection
Given a circular linked list, implement an algorithm that returns the node at
the beginning of the loop.
This method is O(n) in memory, compared to O(1) for the runner method,
but I feel like its simplicity justifies such a cost.*/

#include <iostream>
#include <unordered_set>
#include <list>

namespace {

template <class T>
struct Node {
  T data;
  Node<T> *next;
};

template <class T>
struct List {
  Node<T> *head;
};

template <class T>
Node<T>* DetectLoop(List<T> l) {
  std::unordered_set<Node<T>*> nodes;
  for (Node<T> *it = l.head; ; it = it->next) {
    if (nodes.find(it) != nodes.end())
      return it;
    nodes.insert(it);
  }
}    

template <class T>
Node<T>* DetectLoopRunner(List<T> l) {
  std::cout << "Run.";
  if (l.head == nullptr)
    return l.head;
  Node<T> *runner = l.head, *it1 = l.head, *it2 = l.head;
  runner = ((l.head)->next)->next;
  while (runner != it1) {
    std::cout << "runnin'";
    it1 = it1->next;
    runner = (runner->next)->next;
  }
  while (it1 != it2) {
    it1 = it1->next;
    it2 = it2->next;
  }
  return it1;
}

} // namespace

int main(void) {
  Node<int> n0 = {0, nullptr};
  Node<int> n1 = {1, nullptr};
  Node<int> n2 = {2, nullptr};
  Node<int> n3 = {3, nullptr};
  n0.next = &n1;
  n1.next = &n2;
  n2.next = &n3;
  n3.next = &n1;
  List<int> l = {&n0};
  std::cout << &n1 << std::endl;
  std::cout << DetectLoop<int>(l) << std::endl;
  std::cout << (l.head->next)->data << std::endl;
  std::cout << DetectLoopRunner<int>(l) << std::endl;
  return 0;
}
