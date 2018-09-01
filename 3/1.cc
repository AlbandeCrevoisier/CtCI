/* Three In One
Describe how you could use a single array to implement three stacks.
Note: used a vector, not an array. For an array, same code with a check to see
when we exceed the allocated space, fixed amount of n/3 per stack. */

#include <iostream>
#include <vector>

namespace {

const int kNumberOfStacks = 3;

struct Stack {
  std::vector<int> *array;
  int root_index;
  int offset;
};

int PushStack(Stack &s, int val) {
  while (int((*s.array).size())  < s.root_index + s.offset*kNumberOfStacks + 1)
    (*s.array).push_back(0);
  (*s.array)[s.root_index + s.offset*kNumberOfStacks] = val;
  s.offset++;
  return 0;
}

int PopStack(Stack &s, int *ret) {
  if (s.offset == 0)
    return -1;
  s.offset--;
  *ret = (*s.array)[s.root_index + s.offset*kNumberOfStacks];
  return 0;
}

int GetStackHead(Stack s, int *head) {
  if (s.offset == 0)
    return -1;
  *head = (*s.array)[s.root_index + (s.offset-1)*kNumberOfStacks];
  return 0;
}

bool IsEmpty(Stack s) {
  return s.offset == 0;
}

} // namespace

int main(void) {
  std::vector<int> a;
  Stack s = {&a, 1, 0};
  PushStack(s, 42);
  PushStack(s, 9);
  PushStack(s, 1337);
  int ret = 0;
  GetStackHead(s, &ret);
  std::cout << ret << std::endl;
  while (!IsEmpty(s)) {
    PopStack(s, &ret);
    std::cout << ret << ":";
  }
  std::cout << std::endl;
  return 0;
}
