/* Stack of Plates
Imagine a stack of plates. If the stack gets too high, it might topple.
Therefore, in real life, we would likely start a new stack when the previous
stack exceeds some threshold. Implement a data structure SetOfStacks that mimics
this.
Follow up: Implement a function PopAt(int index) which performs a pop operation
on a specific sub-stack. */

#include <iostream>
#include <stack>
#include <list>

namespace {

struct SetOfStacks {
  std::stack<std::stack<int>> s;
  int kThreshold;
};

void Push(SetOfStacks &sos, int val) {
  if (sos.s.empty() || int(sos.s.top().size()) == sos.kThreshold) {
    std::stack<int> new_stack;
    sos.s.push(new_stack);
  }
  sos.s.top().push(val);
}

void Pop(SetOfStacks &sos) {
  if (sos.s.empty())
    return;
  sos.s.top().pop();
  if (sos.s.top().empty())
    sos.s.pop();
}

int Top(SetOfStacks sos) {
  return sos.s.top().top();
}

/* Follow up */
/* TODO: find a better name */
struct SetOfStacksBis {
  std::list<std::list<int>> stacks;
  int kThreshold;
};

void PopAt(SetOfStacksBis &sosb, int stack_index) {
  if (sosb.stacks.size() <= uint(stack_index)) {
    return;
  } else {
    auto it = sosb.stacks.begin();
    for (int i = 0; i < stack_index; i++)
      it++;
    (*it).erase((*it).end());
    if ((*it).empty()) {
      sosb.stacks.erase(it);
      return;
    }
    auto it_temp = it++;
    while (it != sosb.stacks.end()) {
      (*it_temp).emplace_back((*it).front());
      (*it).erase((*it).begin());
      if ((*it).empty()) {
        sosb.stacks.erase(it);
        return;
      }
      it++;
      it_temp++;
    }
  }
}

} // namespace

int main(void) {
  std::stack<std::stack<int>> ss;
  SetOfStacks sos = {ss, 1};
  Push(sos, 42);
  Push(sos, 1337);
  std::cout << Top(sos) << std::endl;
  Pop(sos);
  std::list<std::list<int>> sss;
  SetOfStacksBis sosb = {sss, 2};
  PopAt(sosb, 1);
  return 0;
}
