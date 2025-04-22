#include <vector>
#include <algorithm>

struct Edge {
    int u, v, weight;
};

bool detectCycleWithEdge(const std::vector<Edge>& edges, const Edge& newEdge) {
    // Проверка на наличие цикла.
    return false;
}

std::vector<Edge> ALG_3(const std::vector<Edge>& graph, int vertexCount) {
    std::vector<Edge> T;
    for (const Edge& e : graph) {
        T.push_back(e);
        if (detectCycleWithEdge(T, e)) {
            auto maxEdge = max_element(T.begin(), T.end(),
                                       [](const Edge& a, const Edge& b) {
                                           return a.weight < b.weight;
                                       });
            T.erase(maxEdge);
        }
    }
    return T;
}
