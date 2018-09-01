/* Intersection
Given two singly linked lists, determine if the two lists intersect. Return the
intersection node. Note that the intersection is based on reference, not value.
That is, if the kth node of the first linked list is the exact same node (by
reference) as the jth node of the second linked list, then they are inter-
secting. */

#include <iostream>

namespace {

template <class T>
struct Node {
  T data;
  Node *next;
};

template <class T>
struct List {
  Node<T> *head;
};

/* TODO: find a better name. */
struct ReturnStruct {
  bool is_intersect;
  int l1_len;
  int l2_len;
};

template <class T>
ReturnStruct IsIntersect(List<T> l1, List<T> l2) {
  if (l1.head == nullptr || l2.head == nullptr)
    return {false, 0, 0};

  int l1_len = 0, l2_len = 0;
  Node<T> *n1 = l1.head;
  Node<T> *n2 = l2.head;
  while (n1->next != nullptr) {
    n1 = n1->next;
    l1_len++;
  }
  while (n2->next != nullptr) {
    n2 = n2->next;
    l2_len++;
  }
  if (n1 == n2)
    return {true, l1_len, l2_len};
  else
    return {false, 0, 0};
}

template <class T>
Node<T>* GetIntersection(List<T> l1, int l1_len, List<T> l2, int l2_len) {
  Node<T> *n1 = l1.head;
  Node<T> *n2 = l2.head;
  if (l1_len < l2_len)
    for(int i = 0; i < l2_len-l1_len; i++)
      n2 = n2->next;
  else
    for(int i = 0; i < l1_len-l2_len; i++)
      n1 = n1->next;
  while (n1 != n2) {
    n1 = n1->next;
    n2 = n2->next;
  }
  return n1;
}

/* TODO: rename. */
template <class T>
struct ReturnStructBis {
  bool is_intersect;
  Node<T> *intersect;
};

template <class T>
ReturnStructBis<T> FindIntersection(List<T> l1, List<T> l2) {
  ReturnStruct rs = IsIntersect<T>(l1, l2);
  if (!rs.is_intersect)
    return {false, nullptr};
  else
    return {true, GetIntersection<T>(l1, rs.l1_len, l2, rs.l2_len)};
}

} // namespace

int main(void) {
  Node<int> n1 = {1, nullptr};
  Node<int> n2 = {2, nullptr};
  Node<int> n3 = {3, nullptr};
  Node<int> n4 = {4, nullptr};
  Node<int> n5 = {5, nullptr};
  n1.next = &n2;
  n2.next = &n3;
  n3.next = &n4;
  List<int> l1 = {&n1};
  List<int> l2 = {&n3};
  List<int> l3 = {&n5};
  ReturnStructBis<int> ret_true = FindIntersection<int>(l1, l2);
  ReturnStructBis<int> ret_false = FindIntersection<int>(l1, l3);
  std::cout << ret_true.is_intersect << ":" << ret_true.intersect << " ";
  std::cout << ret_false.is_intersect << ":" << ret_false.intersect;
  std::cout << std::endl;

  return 0;
}
