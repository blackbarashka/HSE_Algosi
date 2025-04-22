//Мусаев Умахан.
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

void dfs1(int v, vector<vector<int>> &graph, vector<bool> &visited, stack<int> &finish_stack) {
    visited[v] = true;
    for (int u : graph[v]) {
        if (!visited[u]) {
            dfs1(u, graph, visited, finish_stack);
        }
    }
    finish_stack.push(v);
}

void dfs2(int v, vector<vector<int>> &transposed_graph, vector<bool> &visited, vector<int> &component, int comp_id) {
    visited[v] = true;
    component[v] = comp_id;
    for (int u : transposed_graph[v]) {
        if (!visited[u]) {
            dfs2(u, transposed_graph, visited, component, comp_id);
        }
    }
}

void kosaraju(int n, vector<pair<int, int>> &edges) {
    vector<vector<int>> graph(n);
    vector<vector<int>> transposed_graph(n);

    // Создаем граф и его транспонированный вариант
    for (auto edge : edges) {
        int u = edge.first - 1;
        int v = edge.second - 1;
        graph[u].push_back(v);
        transposed_graph[v].push_back(u);
    }

    // Выполняем первый проход DFS для нахождения порядка завершения
    vector<bool> visited(n, false);
    stack<int> finish_stack;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs1(i, graph, visited, finish_stack);
        }
    }

    // Выполняем второй проход DFS по транспонированному графу
    fill(visited.begin(), visited.end(), false);
    vector<int> component(n, -1);
    int comp_id = 0;

    while (!finish_stack.empty()) {
        int v = finish_stack.top();
        finish_stack.pop();
        if (!visited[v]) {
            dfs2(v, transposed_graph, visited, component, comp_id);
            ++comp_id;
        }
    }

    // Вывод результатов
    cout << comp_id << "\n";
    for (int i = 0; i < n; ++i) {
        cout << component[i] + 1 << " ";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);

    for (int i = 0; i < m; ++i) {
        cin >> edges[i].first >> edges[i].second;
    }

    kosaraju(n, edges);

    return 0;
}
