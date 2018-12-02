/* The Apocalypse
Queen, birth rate: at least one girl per family.
Assuming parent stop having children after their first girl, gender ratio? */

/* Intuition: it should be balanced. n pers : n girls, then n/2 + n/4 + n/8 + ..
boys, which is n*(1-(1/2)^n), which converges to 1. */

#include <iostream>

namespace {

/* Return the % of girls. */
int Apocalypse(int n) {
  int g = n;
  int b = 0;
  for (n /= 2; n > 0; n /= 2)
    b += n;
  return 100 * g / (b + g);
}

} /* namespace */

int main(void) {
  std::cout << Apocalypse(1337) << std::endl;
  return 0;
}

