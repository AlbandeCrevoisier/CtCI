/* Insertion
Two 32-bit numbers, m & n, two bit positions, i & j. Insert m into n from j to i. */

#include <iostream>
#include <cstdint>

namespace {

uint32_t Insert(uint32_t m, uint32_t n, int i, int j) {
  for (int k = i; k <= j; k++) {
    n = (n & ~(1 << k)) | ((m & 1) << k);
    m = m >> 1;
  }
  return n;
}

} // namespace

int main(void) {
  uint32_t m = 253, n = 1048576, i = 2, j = 9;
  uint32_t r = Insert(m, n, i, j);
  std::cout << "m:";
  for (int i = 31; i >= 0; i--)
    std::cout << ((m >> i) & 1);
  std::cout << std::endl;
  std::cout << "n:";
  for (int i = 31; i >= 0; i--)
    std::cout << ((n >> i) & 1);
  std::cout << std::endl;
  std::cout << "r:";
  for (int i = 31; i >= 0; i--)
    std::cout << ((r >> i) & 1);
  std::cout << std::endl;
  return 0;
}

