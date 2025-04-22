#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, weight;
};

bool isConnected(const vector<Edge>& edges, int vertexCount) {
    // Реализация проверки на связность: например, с помощью Union-Find
    return true;
}

vector<Edge> ALG_1(const vector<Edge>& graph, int vertexCount) {
    vector<Edge> edges = graph;
    sort(edges.rbegin(), edges.rend(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    vector<Edge> T = edges;
    for (const Edge& e : edges) {
        T.erase(find(T.begin(), T.end(), e));
        if (!isConnected(T, vertexCount)) {
            T.push_back(e); // вернуть ребро назад
        }
    }
    return T;
}
