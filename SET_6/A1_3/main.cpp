#include <vector>
#include <algorithm>
using namespace std;
struct Edge {
    int u, v, weight;
};

bool detectCycleWithEdge(const vector<Edge>& edges, const Edge& newEdge) {
    // Проверка на наличие цикла.
    return false;
}

vector<Edge> ALG_3(const vector<Edge>& graph, int vertexCount) {
    vector<Edge> T;
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
