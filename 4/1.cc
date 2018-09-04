/* Route Between Nodes
Given a directed graph, design an algorithm to to find out whether there is a
route between two nodes.
Note: *directed* graph. This is for undirected. Easier for directed, just do
one BFS instead of the two. */

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

namespace {

struct Node {
  std::vector<Node*> neighbours;
};

struct Graph {
  std::vector<Node*> nodes;
};

bool RouteBetweenNodes(Node *m, Node *n) {
  std::queue<Node*> m_q, n_q;
  std::unordered_set<Node*> m_visited, n_visited;
  m_visited.insert(m);
  m_q.push(m);
  n_visited.insert(n);
  n_q.push(n);
  while(!m_q.empty() && !n_q.empty()) {
    /* BFS from m */
    Node *m_tmp = m_q.front();
    if (n_visited.find(m_tmp) != n_visited.end())
      return true;
    m_q.pop();
    for (auto neigh : m_tmp->neighbours) {
      if (m_visited.find(neigh) == m_visited.end()) {
        m_visited.insert(neigh);
        m_q.push(neigh);
      }
    }
    /* BFS from n */
    Node *n_tmp = n_q.front();
    if (m_visited.find(n_tmp) != m_visited.end())
      return true;
    n_q.pop();
    for (auto neigh : n_tmp->neighbours) {
      if (n_visited.find(neigh) == n_visited.end()) {
        n_visited.insert(neigh);
        n_q.push(neigh);
      }
    }
  }
  return false;
}

} // namespace

int main(void) {
  Node n1, n2, n3, n4, n5;
  n1.neighbours = {&n2, &n3};
  n2.neighbours = {&n1, &n4};
  n3.neighbours = {&n1, &n4};
  n4.neighbours = {&n2, &n3};
  n5.neighbours = {};

  std::cout << RouteBetweenNodes(&n1, &n2) << std::endl;
  std::cout << RouteBetweenNodes(&n1, &n5) << std::endl;
  
  return 0;
}

