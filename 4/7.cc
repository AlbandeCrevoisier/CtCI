/* Build Order
You are given a list of projects and a list of dependencies (which is a list of
pair of projects, where the second project depends on the first project). All
of the dependencies of a project must be built before the project is. Find a
build order that will allow the projects to be built. If there is no valid build
order, return an error. */

#include <iostream>
#include <vector>
#include <utility>
#include <unordered_set>

namespace {

bool BuildBO(int project,
             std::vector<std::vector<int>> dependencies_graph,
             std::vector<bool> &recursed_from,
             std::vector<bool> &built,
             std::vector<int> &build_order) {
  if (recursed_from[project])
    return false;
  else
    recursed_from[project] = true;
  for (auto p : dependencies_graph[project]) {
    if (!built[p]) {
      if (!BuildBO(p, dependencies_graph, recursed_from, built, build_order))
        return false;
    }
  }
  build_order.push_back(project);
  built[project] = true;
  return true;
}

std::vector<int> BuildOrder(std::vector<int> projects,
                            std::vector<std::pair<int, int>> dependencies) {
  std::vector<std::vector<int>> dependencies_graph;
  for (uint i = 0; i < projects.size(); i++)
    dependencies_graph.push_back({});
  std::vector<bool> is_a_dependency (projects.size(), false);
  for (auto dep : dependencies) {
    dependencies_graph[dep.first].push_back(dep.second);
    is_a_dependency[dep.second] = true;
  }

  std::vector<bool> recursed_from (projects.size(), false);
  std::vector<bool> built (projects.size(), false);
  std::vector<int> build_order = {};
  for (auto p : projects) {
    if (!is_a_dependency[p]) {
      if (!BuildBO(p, dependencies_graph, recursed_from, built, build_order))
        return {};
    }
  }
  return build_order;
}

} // namespace

int main(void) {
  std::vector<int> p = {0, 1, 2, 3, 4, 5};
  std::vector<std::pair<int, int>> dep = {{0,1},{0,2},{1,2},{1,3},{2,3},{2,4}};
  std::vector<int> bo = BuildOrder(p, dep);
  for (auto e : bo)
    std::cout << e << std::endl;
  std::cout << std::endl;
  return 0;
}

