/* Sum Lists
You have two numbers represented by a linked list, where each node contains
a single digit. The digits are stored in reverse order. Write a function
that adds the two numbers and returns the sum as a linked list.
Follow up: digits stored in forward order. */

#include <iostream>
#include <list>

namespace {

std::list<int> SumLists(std::list<int> l1, std::list<int> l2) {
  std::list<int> ret;
  auto it1 = l1.begin();
  auto it2 = l2.begin();
  int carry = false;

  while (it1 != l1.end() && it2 != l2.end()) {
    int sum = (*it1) + (*it2);
    if (carry) {
      sum++;
      carry = false;
    }
    if (sum >=10) {
      carry = true;
      sum -=10;
    }
    ret.emplace_back(sum);
    it1++;
    it2++;
  }
  while (carry) {
    if (it1 == l1.end() && it2 == l2.end()) {
      ret.emplace_back(1);
      carry = false;
    } else {
      carry = false;
      int sum = (it1 == l1.end() ? (*it2) : (*it1));
      sum++;
      if (sum >=10) {
        carry = true;
        sum -=10;
      }
      ret.emplace_back(sum);
      if (it1 != l1.end())
        it1++;
      else
        it2++;
    }
  }
  if (it1 != l1.end())
    ret.insert(ret.end(), it1, l1.end());
  else if (it2 != l2.end())
    ret.insert(ret.end(), it2, l2.end());
  return ret;
}

std::list<int> SumListsForward(std::list<int> l1, std::list<int> l2) {
  std::list<int> l1_back, l2_back, ret, ret_back;
  for (auto e : l1)
    l1_back.emplace_front(e);
  for (auto e : l2)
    l2_back.emplace_front(e);
  ret = SumLists(l1_back, l2_back);
  for (auto e : ret)
    ret_back.emplace_front(e);
  return ret_back;
}

/* Recursion */
struct Node {
  int data;
  Node *next_node;
};

int Carry(Node *n1, Node *n2, Node *ret) {
  if (n1 == nullptr)
    return 0;
  int carry = Carry(n1->next_node, n2->next_node, ret->next_node);
  int sum = n1->data + n2->data + carry;
  ret->data = sum % 10;
  if (sum >= 10)
    return 1;
  else
    return 0;
}

} // namespace

int main(void) {
  std::list<int> l1 = {0, 1, 2};
  std::list<int> l2 = {3, 9, 2};
  std::list<int> ret = SumLists(l1, l2);
  for (auto e : ret)
    std::cout << e << ":";
  std::cout << std::endl;
  ret = SumListsForward(l1, l2);
  for (auto e : ret)
    std::cout << e << ":";
  std::cout << std::endl;
  Node n1 = {5, nullptr};
  Node n2 = {9, nullptr};
  Node n3 = {0, nullptr};
  std::cout << Carry(&n1, &n2, &n3) << n3.data << std::endl;
  return 0;
}
