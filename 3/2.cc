/* Stack Min
How would you design a stack which, in addition to push and pop, has a function
min which returns the minimum element? Push, pop, and min should all operate
in O(1) time. */

#include <iostream>
#include <stack>

namespace {

struct StackMin {
  std::stack<int> stack;
  std::stack<int> mins;
};

void Push(StackMin &s, int val) {
  s.stack.push(val);
  if (s.mins.empty())
    s.mins.push(val);
  else if (val < s.mins.top())
    s.mins.push(val);
}

void Pop(StackMin &s) {
  if (s.stack.top() == s.mins.top())
    s.mins.pop();
  s.stack.pop();
}

int Min(StackMin s) {
  return s.mins.top();
}

int Top(StackMin s) {
  return s.stack.top();
}

} //namespace

int main(void) {
  std::stack<int> s, m;
  StackMin sm = {s, m};
  Push(sm, 42);
  Push(sm, 1337);
  Push(sm, 9);
  std::cout << Min(sm) << std::endl;
  Pop(sm);
  std::cout << Top(sm) << std::endl;
  return 0;
}
