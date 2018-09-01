/* Sort Stack
Sort a stack: smallest on top. You can use a temporary stack. */

#include <iostream>
#include <stack>
#include <climits>

namespace {

void SortStack(std::stack<int> &s) {
  int length = int(s.size());
  while (length != 0) {
    std::stack<int> buf;
    int counter;
    int max = INT_MIN;
    for (int i = 0; i < length; i++) {
      int temp = s.top();
      s.pop();
      buf.push(temp);
      if (temp > max) {
        counter = 1;
        max = temp;
      } else if (temp == max) {
        counter++;
      }
    }
    for (int i = 0; i < counter; i++)
      s.push(max);
    length -= counter;
    while(!buf.empty()) {
      if (buf.top() != max)
        s.push(buf.top());
      buf.pop();
    }
  }
}

std::stack<int> InsertSortStack(std::stack<int> s) {
  std::stack<int> ret;
  while (!s.empty()) {
    int temp = s.top();
    s.pop();
    int counter = 0;
    while (!ret.empty() && ret.top() < temp) {
      counter++;
      s.push(ret.top());
      ret.pop();
    }
    ret.push(temp);
    for (int i = 0; i < counter; i++) {
      ret.push(s.top());
      s.pop();
    }
  }
  return ret;
}

} // namespace

int main(void) {
  std::stack<int> s;
  s.push(4);
  s.push(2);
  s.push(1);
  s.push(9);
  std::stack<int> t = InsertSortStack(s);
  SortStack(s);
  while (!s.empty()) {
    std::cout << s.top() << ":";
    s.pop();
  }
  std::cout << std::endl;
  while (!t.empty()) {
    std::cout << t.top() << ":";
    t.pop();
  }
  std::cout << std::endl;

  return 0;
}
