/* Next number
Given a positive integer, print the next smallest and the next largest number
that have the same number of 1s in their binary representation.
Assumption: 32-bit integers.
Note: this is actually terribly false, I dived too fast before thinking enough.
Let that be a lesson.*/

#include <iostream>
#include <cstdint>

namespace {

struct Pair {
  uint32_t min, max;
};

void NextNumber(uint32_t u, Pair &p) {
  if (u == 0 || ~u == 0) {
    p.min = u;
    p.max = u;
  }

  p.min = 0;
  p.max = 0;
  int t = u;
  while ((t & 1) == 1)
    t >>= 1;
  if (t == 0)
    p.min = u;
  t = u;
  while ((t & (1 << 31)) == 1)
    t <<= 1;
  if (t == 0)
    p.max = u;

  t = u;
  if (p.min == 0) {
    int i;
    for (i = 0; i < 32; i++) {
      if ((t & 1) == 0 && ((t >> 1) & 1) == 1)
        break;
      t >>= 1;
    }
    p.min = (u & ~(1 << i + 1)) | (1 << i);
  }
  t = u;
  if (p.max == 0) {
    int i;
    for (i = 0; i < 32; i++) {
      if ((t & 1) == 1 && ((t >> 1) & 1) == 0)
        break;
      t >>= 1;
    }
    p.max = (u & ~(1 << i)) | (1 << i + 1);
  }
}

} //namespace

int main(void) {
  Pair p;
  NextNumber(1337, p);
  std::cout << p.min << ":" << p.max << std::endl;
  return 0;
}

