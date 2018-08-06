/* String Compression
Implement a method to perform basic string compression using the counts
of repeated characters. Ex: aabcccccaaa -> a2b1c5a3. If the "compressed"
string would not become smaller than the original string, your method
should return the original string. You can assume the string has only
uppercase or lowercase letters. */

#include <iostream>
#include <string>

namespace {

std::string CompressString(std::string s) {
  std::string ret_str = "";

  int char_count = 1;
  for (uint i = 0; i < s.size(); i++) {
    if (i < s.size()-1 && s[i] == s[i+1]) {
      char_count++;
    } else {
      ret_str.push_back(s[i]);
      ret_str += std::to_string(char_count);
      if (ret_str.size() > s.size())
        return s;
      char_count = 1;
    }
  }

  return ret_str;
}

} // namespace

int main(void) {
  std::string s = "aabcccccaaa", t = "abcde";
  std::cout << CompressString(s) << std::endl;
  std::cout << CompressString(t) << std::endl;
  return 0;
}
