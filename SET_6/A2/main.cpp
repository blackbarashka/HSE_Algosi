#include <vector>
#include <queue>
#include <limits>
using namespace std;
const double INF = numeric_limits<double>::infinity();
vector<double> DijkstraMULT(const vector<vector<pair<int, double>>>& graph, int start) {
    int n = graph.size();
    vector<double> dist(n, INF);
    dist[start] = 1; // Начинаем с единицы.

    using State = pair<double, int>;
    priority_queue<State, vector<State>, greater<State>> min_heap;
    min_heap.push({1, start});

    while (!min_heap.empty()) {
        double current_dist = min_heap.top().first;
        int u = min_heap.top().second;
        min_heap.pop();

        if (current_dist > dist[u]) continue;

        for (const auto& edge : graph[u]) {
            int v = edge.first;
            double weight = edge.second;
            double new_dist = current_dist * weight; // Умножение.

            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                min_heap.push({new_dist, v});
            }
        }
    }
    return dist;
}
