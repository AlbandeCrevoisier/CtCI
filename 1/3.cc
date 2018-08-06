/* URLify
Write a method to replace all spaces in a string with '%20'. You may
assume that the string has sufficient space at the end to hold the additional
characters, and that you are given the "true" length of the string. */

#include <string>
#include <iostream>

namespace {

int URLifySpaces(std::string &s, int len) {
  for (int i = len-1; i >= 0; i--) {
    if (s[i] == ' ') {
      for (int j = len+1; j > i+2; j--)
        s[j] = s[j-2];
      s[i] = '%';
      s[i+1] = '2';
      s[i+2] = '0';
      len += 2;
    }
  }
  return 0;
}

} // namespace

int main(void) {
  std::string s = "Mr A C    ";
  int l = 6;
  URLifySpaces(s, l);
  std::cout << s << std::endl;
  return 0;
}
