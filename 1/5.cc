/* One Away
There are three types of edits that can be performed on strings: insert
a character, remove a character, or replace a character. Given two strings,
write a function to check if they are one (or zero) edit away.
Correction: it is possible to merge all code in one function where we
check the nature of the difference instead of checking for replacement
& insert/remove seperately.*/

#include <iostream>
#include <string>

namespace {

bool IsLessThanOneEditAway(std::string s, std::string t) {
  if (t.size() < s.size()) {
    std::string tmp = s;
    s = t;
    t  = tmp;
  }
  if (t.size() - s.size() > 1)
    return false;

  if (s.size() == t.size()) {
    int num_edits = 0;
    for (uint i = 0; i < s.size(); i++) {
      if (s[i] != t[i])
        num_edits++;
      if (num_edits > 1)
        return false;
    }
    return true;
  }

  uint j = 0;
  for (uint i = 0; i < s.size(); i++) {
    if (s[i] != t[j]) {
      if (i != j)
        return false;
      j++;
    }
    j++;
  }
  return true;
}

} // namespace

int main(void) {
  std::string s = "abc";
  std::string t[3] = {"aac", "ac", "abdc"};
  std::string f[3] = {"abcde", "", "zzz"};
  for (auto str : t)
    std::cout << IsLessThanOneEditAway(s, str) << ":";
  std::cout << std::endl;
  for (auto str : f)
    std::cout << IsLessThanOneEditAway(s, str) << ":";
  std::cout << std::endl;

  return 0;
}

