/* Return Kth to Last
Implement an algorithm to find the Kth to last element of a singly linked list. */

#include <iostream>
#include <list>

namespace {

int KthToLast(std::list<int> &l, int k, int &kth) {
  if (k < 0)
    return -1;

  auto it = l.begin(), jt = l.begin();
  for (int i = 0; i < k; i++) {
    it++;
    if (it == l.end())
      return -2;
  }
  while (it != l.end()) {
    it++;
    jt++;
  }
  kth = *jt;
  return 0;
}    

} // namespace

int main(void) {
  std::list<int> l = {0,1,2,3,4,5,6,7};
  int k = 3, kth;
  int ret = KthToLast(l, k, kth);
  for (auto e : l)
    std::cout << e << ":";
  std::cout << std::endl;
  std::cout << k << std::endl;
  if (ret < 0)
    std::cout << "Error:" << ret << std::endl;
  else
    std::cout << kth << std::endl;
  return 0;
}

