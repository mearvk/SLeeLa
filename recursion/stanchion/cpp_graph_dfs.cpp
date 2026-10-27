// Applied recursion: graph DFS with visited-state protection against cycles.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic cpp_graph_dfs.cpp -o cpp_graph_dfs
#include <iostream>
#include <vector>

void dfs(std::size_t vertex, const std::vector<std::vector<std::size_t>>& graph,
         std::vector<bool>& visited) {
    if (vertex >= graph.size() || visited[vertex]) return;
    visited[vertex] = true;
    std::cout << vertex << ' ';
    for (const std::size_t next : graph[vertex])
        if (next < graph.size()) dfs(next, graph, visited);
}

int main() {
    const std::vector<std::vector<std::size_t>> graph{
        {1, 2}, {0, 3}, {0, 3}, {1, 2, 4}, {3}
    };
    std::vector<bool> visited(graph.size(), false);
    std::cout << "DFS from vertex 0: ";
    dfs(0U, graph, visited);
    std::cout << '\n';
}
