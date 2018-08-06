/* Rotate Matrix
Given an image represented by an NxN matrix, where each pixel in the image
is 4 bytes, write a method to rotate the image by 90 degrees. Can you do
this in place?
Assumption: rotate clockwise. */

#include <iostream>
#include <cstdint>
#include <vector>

namespace {

int RotateMatrix(std::vector<std::vector<int16_t>> &mat) {
  uint len = mat.size();
  uint n = len-1;
  for (uint i = 0; i < len/2; i++) {
    for (uint j = 0; j < len/2; j++) {
      int tmp = mat[n-j][i];
      mat[n-j][i] = mat[n-i][n-j];
      mat[n-i][n-j] = mat[j][n-i];
      mat[j][n-i] = mat[i][j];
      mat[i][j] = tmp;
    }
  }

  if (len % 2 == 1) {
    uint half = len/2;
    for (uint i = 0; i < half; i++) {
      int tmp = mat[half][i];
      mat[half][i] = mat[n-i][half];
      mat[n-i][half] = mat[half][n-i];
      mat[half][n-i] = mat[i][half];
      mat[i][half] = tmp;
    }
  }
  return 0;
}

} // namespace

int main(void) {
  std::vector<std::vector<int16_t>> e = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
  RotateMatrix(e);
  for (auto w : e) {
    for (auto p : w)
      std::cout << p;
    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::vector<std::vector<int16_t>> o = {{0, 1}, {2, 3}};
  RotateMatrix(o);
  for (auto w : o) {
    for (auto p : w)
      std::cout << p;
    std::cout << std::endl;
  }
  std::cout << std::endl;

  return 0;
}

