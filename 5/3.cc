/* Flip Bit to Win
Integer, you can flip one bit from a 0 to a 1. Longest sequence of 1s you can
make?
Note: the problem is so simple and restrained, that the naive solution seems
to be a reasonable answer. */

#include <iostream>

namespace {

/* Computes the longest sequence of 1 in an integer. */
int Longest1(int m) {
  int s = (int) 8 * sizeof(m);
  int longest = 0, l = 0;
  for (int i = 0; i <= s; i++) {
    if (((m >> i) & 1) == 1) {
    l++;
  } else {
    if (l > longest)
      longest = l;
      l = 0;
    }
  }
  return longest;
}

int FlipBittoWin(int m) {
  int s = (int) 8 * sizeof(m);
  if (m == -1)
  return s;
  int longest = 0, l = 0;
  for (int i = 0; i < s; i++) {
    if (((m >> i) & 1) == 0) {
      l = Longest1(m | (1 << i));
      if (l > longest)
        longest = l;
      }
  }
  return longest;
}

} // namespace

int main(void) {
  int m = 1012;
  std::cout << "m:";
  for (int i = 31; i >= 0; i--)
    std::cout << ((m >> i) & 1);
  std::cout << std::endl;
  std::cout << FlipBittoWin(1012) << std::endl;
  return 0;
}

