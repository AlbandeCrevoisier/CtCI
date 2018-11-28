/* Next number
Given a positive integer, print the next smallest and the next largest number
that have the same number of 1s in their binary representation.
Assumption: 32-bit integers. */

#include <iostream>
#include <cstdint>

namespace {

uint32_t NextNumber(uint32_t n) {
  uint32_t m = n;
  while ((m & 1 << 31) == (1 << 31))
    m <<= 1;
  if (m == 0)
    return n;

  /* bit 0:i-1 0s
   * bit i:j-1 1s
   * Change 0(1..1)(0..0) into 1(0..0)(1..1) */
  m = n;
  int i = 0, j = 0;
  while (m & 1 == 0) {
    i++;
    m >>= 1;
  }
  while (m & 1 == 1) {
    j++;
    m >>= 1;
  }
  n += 1 << i;
  for (int k = 0; k < j-1; k++)
    n |= 1 << k;
  return n;
}

uint32_t PrevNumber(uint32_t n) {
  uint32_t m = n;
  while (m & 1 == 1)
    m >>= 1;
  if (m == 0)
    return n;

  /* bit 0:i-1 1s
   * bit i:j-1 0s
   * Change 1(0..0)(1..1) into 0(1..1)(0..0) */
  m = n;
  int i = 0, j = 0;
  while (m & 1 == 1) {
    m >>= 1;
    i++;
  }
  while (m & 1 == 0) {
    m >>= 1;
    j++;
  }
  n += 1;
  n -= 1 << (i + 1);
  return n;
}

void PrevNextNumbers(uint32_t n) {
  std::cout << "Next:" << NextNumber(n) << std::endl;
  std::cout << "Prev:" << PrevNumber(n) << std::endl;
}

} //namespace

int main(void) {
  PrevNextNumbers(1337);
  return 0;
}

