/* Remove Dups
Write code to remove duplicates from an unsorted linked list.
Follow up: how would you solve this problem if a temporary buffer is
not allowed?
Assumption: list of integers.*/

#include <iostream>
#include <list>
#include <iterator>
#include <unordered_set>

namespace {

int RemoveDups(std::list<int> &l) {
  if (l.empty())
    return 0;
  std::unordered_set<int> elements;
  for (auto it = l.begin(); it != l.end(); it++) {
    if (elements.find(*it) != elements.end()) {
      it = l.erase(it);
      it--;
    } else {
      elements.insert(*it);
    }
  }
  return 0;
}

int RemoveDupsNoBuff(std::list<int> &l) {
  if (l.empty())
    return 0;
  for (auto it = l.begin(); it != l.end(); it++) {
    for (auto jt = std::next(it); jt != l.end(); jt++) {
      if (*it == *jt) {
        it = l.erase(it);
        it--;
      }
    }
  }
  return 0;
}

} // namespace

int main(void) {
  std::list<int> l = {1, 1, 2, 3, 3, 4, 4, 4, 5};
  std::list<int> m = {1, 1, 2, 3, 3, 4, 4, 4, 5};
  RemoveDups(l);
  for (auto e : l)
    std::cout << e;
  std::cout << std::endl;
  RemoveDupsNoBuff(m);
  for (auto e : m)
    std::cout << e;
  std::cout << std::endl;
  return 0;
}

