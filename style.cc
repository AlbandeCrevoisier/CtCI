/* This file showcases Google's c++ coding style. */

#include "style.h"

namespace style {
} // namespace style

namespace {

struct TypeNamingConvention {};
enum TypeNamingConvention {
  kFirstConstant = 2;
  kSecond;
  kThird;
};
int variable_naming_convention = 0;
const int kConstantNamingConvention = 42;
int FunctionNamingConvention(void);

/* Showcases function coding style */
int FunctionNamingconvention(void) {
}

} // namespace
