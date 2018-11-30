/* Draw Line
Monochrome screen: single array of bytes, 8 pixels / byte, width w = 8 * k.
Draw a horizontal line (x1, y), (x2, y).
Assumptions: x from left to right, y from top to bottom, screen from top left.*/

#include <iostream>

namespace {

void NaiveDrawLine(char screen[], int width, int x1, int x2, int y) {
  int a, b;
  if (x1 < x2) {
    a = x1;
    b = x2;
  } else {
    a = x2;
    b = x1;
  }
  int w = width / 8;
  for (int i = a; i <= b; i++) {
    screen[w*y + i/8] |= 1 << 7 - i%8;
  }
}

void DrawLine(char screen[], int width, int x1, int x2, int y) {
  int a, b;
  if (x1 <= x2) {
    a = x1;
    b = x2;
  } else {
    a = x2;
    b = x1;
  }
  int w = width / 8;

  if (a/8 == b/8)
    screen[w*y + a/8] |= (0xFF >> a%8) & ~(0xFF >> b%8 + 1);
    
  for (int i = a/8 + 1; i < b/8; i++)
    screen[w*y + i/8] = 0xFF;
  screen[w*y + a/8] |= 0xFF >> a%8;
  screen[w*y + b/8] |= ~(0xFF >> b%8 + 1);
}

} // namespace

int main(void) {
  char s[] = {0, 0, 0};
  std::cout << (int)s[0] << ":" << (int)s[1] << ":" << (int)s[2]  << std::endl;
  NaiveDrawLine(s, 3*8, 2, 10, 0);
  std::cout << (int)s[0] << ":" << (int)s[1] << ":" << (int)s[2]  << std::endl;
  DrawLine(s, 3*8, 2, 10, 0);
  std::cout << (int)s[0] << ":" << (int)s[1] << ":" << (int)s[2]  << std::endl;
  return 0;
}

