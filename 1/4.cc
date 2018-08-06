/* Palindrome Permutation
Given a string, write a function to check if it is a permutation of a
palindrome. 
Alternative solution: charset vector, count frequencies, set to 0 when it
reaches 2, then sum & check vs 0 or 1.
Better: bit vector. Swag: i == 0 or i & (i-1) == 0.*/

#include <iostream>
#include <string>
#include <unordered_map>

namespace {

bool IsPermutatedPalindrome(std::string s) {
  if (s.empty())
    return true;

  /* Count characters. */
  std::unordered_map<char, int> num_chars;
  for (auto c : s) {
    if (num_chars.find(c) == num_chars.end())
      num_chars.insert({c, 1});
    else
      num_chars[c]++;
  }

  int num_chars_odd_num = 0;
  for (auto c : s)
    if (num_chars[c] % 2 == 1)
      num_chars_odd_num++;
  if (num_chars_odd_num <= 1)
    return true;
  return false;
}

} // namespace

int main(void) {
  std::string s = "aabbc", t = "abc", u = "aacbbdddd", v = "aaaccb";
  std::cout << s << IsPermutatedPalindrome(s) << std::endl;
  std::cout << t << IsPermutatedPalindrome(t) << std::endl;
  std::cout << u << IsPermutatedPalindrome(u) << std::endl;
  std::cout << v << IsPermutatedPalindrome(v) << std::endl;

  return 0;
}
