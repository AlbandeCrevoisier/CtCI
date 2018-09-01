/* Queue via Stacks
Implement a queue using two stacks.
Note: this is actually sort of a single stack implementation. */

#include <iostream>
#include <stack>

namespace {

struct QueueViaStacks {
  std::stack<int> data;
};

void Enqueue(QueueViaStacks &q, int val) {
  q.data.push(val);
}

int Dequeue(QueueViaStacks &q, int *ret) {
  if (q.data.empty())
    return -1;
  std::stack<int> buffer;
  int q_length = int(q.data.size());
  for (int i = 1; i < q_length; i++) {
    buffer.push(q.data.top());
    q.data.pop();
  }
  *ret = q.data.top();
  q.data.pop();
  while(!buffer.empty()) {
    q.data.push(buffer.top());
    buffer.pop();
  }
  return 0;
}

/* Proper double stacks implementation. */
struct Queue {
  std::stack<int> stack_order;
  std::stack<int> queue_order;
};

void EnQueue(Queue &q, int val) {
  q.stack_order.push(val);
}

int DeQueue(Queue &q, int *ret) {
  if (q.queue_order.empty()) {
    if (q.stack_order.empty())
      return -1;
    while (!q.stack_order.empty()) {
      q.queue_order.push(q.stack_order.top());
      q.stack_order.pop();
    }
  }
  *ret = q.queue_order.top();
  q.queue_order.pop();
  return 0;
}

} // namespace

int main(void) {
  std::stack<int> d;
  QueueViaStacks q = {d};
  for (int i = 0; i < 10; i++)
    Enqueue(q, i);
  int ret;
  for (int i = 0; i < 10; i++) {
    Dequeue(q, &ret);
    std::cout << ret << ":";
  }
  std::cout << std::endl;

  std::stack<int> s, t;
  Queue r = {s, t};
  for (int i = 0; i < 10; i++)
    EnQueue(r, i);
  for (int i = 0; i < 10; i++) {
    DeQueue(r, &ret);
    std::cout << ret << ":";
  }
  std::cout << std::endl;

  return 0;
}
