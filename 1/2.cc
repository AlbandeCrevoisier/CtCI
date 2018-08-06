/* Check Permutations
Given two strings, write a method to decide if one is the permutation
of the other. */

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

namespace {

/* Assumption: ASCII */
bool IsPermutationASCII(std::string str1, std::string str2) {
  if (str1.size() != str2.size())
    return false;

  std::vector<int> char_set(128);
  for (auto c : str1)
    char_set[int{c}]++;
  for (auto c : str2)
    char_set[int{c}]--;
  for (auto char_num : char_set)
    if (char_num != 0)
      return false;
  return true;
}

/* No assumption. */
bool IsPermutation(std::string str1, std::string str2) {
  if (str1.size() != str2.size())
    return false;

  std::unordered_map<char, int> chars;
  for (auto c : str1)
    if (chars.find(c) == chars.end())
      chars.insert({c, 1});
    else
      chars[c]++;
  for (auto c : str2)
    if (chars.find(c) == chars.end())
      return false;
    else
      chars[c]--;
  for (auto p : chars)
    if (p.second != 0)
      return false;
  return true;
}

} // namespace

int main(void) {
  std::cout << IsPermutationASCII(std::string("hello"), std::string("olleh"));
  std::cout << IsPermutation(std::string("hello"), std::string("olleh"));
  std::cout << std::endl;
  return 0;
}
