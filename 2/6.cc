/* Palindrome
Implement a function to check if a linked list is a palindrome. */

#include <iostream>
#include <list>

namespace {

template <typename T>
bool IsPalindrome(std::list<T> l) {
  std::list<T> l_reverse = l;
  l_reverse.reverse();
  auto it_l = l.begin();
  auto it_l_reverse = l_reverse.begin();
  for (uint i = 0; i < l.size()/2; i++) {
    if (*it_l != *it_l_reverse)
      return false;
    it_l++;
    it_l_reverse++;
  }
  return true;
}

} // namespace

int main(void) {
  std::list<int> l_true_odd = {3,1,0,1,3};
  std::list<int> l_true_even = {3,1,0,0,1,3};
  std::list<int> l_false = {1,2,3,4,5,6};
  std::cout << IsPalindrome<int>(l_true_odd) << ":true" << std::endl;
  std::cout << IsPalindrome<int>(l_true_even) << ":true" << std::endl;
  std::cout << IsPalindrome<int>(l_false) << ":false" << std::endl;
  return 0;
}
