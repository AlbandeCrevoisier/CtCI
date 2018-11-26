/* Conversion
Write a function to determine how many bits you would need to flip to convert
an integer a to integer b.
Assumption: uint32_t */

#include <iostream>
#include <cstdint>

namespace {

int NBitsDiff(uint32_t a, uint32_t b) {
  a ^= b;
  int count = 0;
  for (int i = 0; i < 32; i++) {
    if ((a & 1) == 1)
      count++;
    a >>= 1;
  }
  return count;
}  

} //namespace

int main(void) {
  std::cout << NBitsDiff(29, 15) << std::endl;
  return 0;
}

