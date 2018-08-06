/* Zero Matrix
Write an algorithm such that if an element in an MxN matrix is 0,
its entire row & column are set to 0.*/

#include <iostream>
#include <vector>

namespace {

int ZeroMatrix(std::vector<std::vector<int>> &mat) {
  uint m = mat.size();
  if (m == 0)
    return 0;
  uint n = mat[0].size();
  if (n == 0)
    return 0;

  for (uint i = 0; i < m; i++) {
    for (uint j = 0; j < n; j++) {
      if (mat[i][j] == 0) {
        mat[i][0] = 0;
        mat[0][j] = 0;
      }
    }
  }
  for (uint j = 1; j < n; j++)
    if (mat[0][j] == 0)
      for (uint i = 0; i < m; i++)
        mat[i][j] = 0;
  for (uint i = 0; i < m; i++)
    if (mat[i][0] == 0)
      mat[i] = std::vector<int> (n, 0);
  if (mat[0][0] == 0)
    for (uint i = 0; i < m; i++)
      mat[i][0] = 0;

  return 0;
}

} // namespace

int main(void) {
  std::vector<std::vector<int>> m = {{0,1,2}, {3,4,0}, {6,7,8}};
  ZeroMatrix(m);
  for (auto v : m) {
    for (auto i : v)
      std::cout << i;
    std::cout << std::endl;
  }

  return 0;
}

