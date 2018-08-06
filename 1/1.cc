/* Is Unique
Implement an algorithm to determine if a string has all unique characters.
What if you cannot use additional data structures? */

#include <iostream>
#include <string>
#include <unordered_set>

namespace {

/* Assumption: unicode. If ASCII, check vs charset length, use vector<bool> of charset. */
bool AreCharUnique(std::string s) {
  std::unordered_set<char> char_hash;
  for (auto c : s) {
    if (char_hash.find(c) != char_hash.end())
      return false;
    char_hash.insert(c);
  }
  return true;
}

/* No other data structure. */
bool AreCharUniqueBis(std::string s) {
  for (uint i = 0; i < s.size(); i++)
    for (uint j = i+1; j < s.size(); j++)
      if (s[i] == s[j])
        return false;
  return true;
}

/* Other solution: sort then compare with the next element. */

} // namespace

int main(void) {
  for (;;) {
    std::string in_string;
    std::cin >> in_string;

    if (AreCharUnique(in_string))
      std::cout << "Unique chars." << std::endl;
    else
      std::cout << "At least one repetition." << std::endl;

    if (AreCharUniqueBis(in_string))
      std::cout << "Unique chars." << std::endl;
    else
      std::cout << "At least one repetition." << std::endl;
  }
  return 0;
}
