#include <vector>
#include <algorithm>

struct Edge {
    int u, v, weight;
};

int findParent(std::vector<int>& parent, int v) {
    if (parent[v] != v)
        parent[v] = findParent(parent, parent[v]);
    return parent[v];
}

bool unionSets(std::vector<int>& parent, std::vector<int>& rank, int u, int v) {
    int pu = findParent(parent, u);
    int pv = findParent(parent, v);
    if (pu == pv) return false;
    if (rank[pu] < rank[pv]) std::swap(pu, pv);
    parent[pv] = pu;
    if (rank[pu] == rank[pv]) rank[pu]++;
    return true;
}

std::vector<Edge> ALG_2(const std::vector<Edge>& graph, int vertexCount) {
    std::vector<Edge> T;
    std::vector<int> parent(vertexCount), rank(vertexCount, 0);

    for (int i = 0; i < vertexCount; ++i)
        parent[i] = i;

    for (const Edge& e : graph) {
        if (unionSets(parent, rank, e.u, e.v)) {
            T.push_back(e);
        }
    }
    return T;
}
