//Musaev Umahan.
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstring>
#include <limits.h>

using namespace std;

struct Edge {
    int u, v, w;
    bool inMST; // Flag to indicate if edge is in MST
};

bool cmp(const Edge &a, const Edge &b) {
    return a.w < b.w;
}

int parent[105];

int find(int u) {
    if (parent[u] != u)
        parent[u] = find(parent[u]); // Path compression
    return parent[u];
}

bool unite(int u, int v) {
    int u_root = find(u);
    int v_root = find(v);
    if (u_root != v_root) {
        parent[v_root] = u_root;
        return true;
    }
    return false;
}

vector<Edge> edges;
vector<pair<int, int>> mst_adj[105]; // Adjacency list for MST (vertex, weight)

int N, M;

// Function to find maximum edge weight on path from u to v in MST
int maxEdgeInPath(int u, int v) {
    bool visited[105];
    memset(visited, false, sizeof(visited));
    queue<pair<int, int>> q; // pair of (node, max_weight)
    q.push({u, 0});
    visited[u] = true;
    while (!q.empty()) {
        int curr = q.front().first;
        int curr_max = q.front().second;
        q.pop();
        if (curr == v) {
            return curr_max;
        }
        for (auto &adj : mst_adj[curr]) {
            int next_node = adj.first;
            int weight = adj.second;
            if (!visited[next_node]) {
                visited[next_node] = true;
                int new_max = max(curr_max, weight);
                q.push({next_node, new_max});
            }
        }
    }
    return -1; // Should not reach here if MST is connected
}

int main() {
    cin >> N >> M;
    edges.resize(M);
    for (int i = 0; i < M; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        edges[i].u = a - 1; // Converting to 0-based index
        edges[i].v = b - 1;
        edges[i].w = c;
        edges[i].inMST = false;
    }

    // Kruskal's algorithm to find MST
    sort(edges.begin(), edges.end(), cmp);
    for (int i = 0; i < N; ++i)
        parent[i] = i;

    int mst_cost = 0;
    vector<Edge> mst_edges;
    for (Edge &e : edges) {
        if (unite(e.u, e.v)) {
            e.inMST = true;
            mst_cost += e.w;
            mst_edges.push_back(e);
            // Build adjacency list for MST
            mst_adj[e.u].push_back({e.v, e.w});
            mst_adj[e.v].push_back({e.u, e.w});
        }
    }

    int second_mst_cost = INT_MAX;
    bool found_equal = false;

    // For each edge not in MST, try to find second MST
    for (Edge &e : edges) {
        if (!e.inMST) {
            int max_w = maxEdgeInPath(e.u, e.v);
            if (max_w == -1) continue; // Not connected, should not happen
            int temp_cost = mst_cost + e.w - max_w;
            if (temp_cost == mst_cost) {
                found_equal = true;
            } else if (temp_cost > mst_cost && temp_cost < second_mst_cost) {
                second_mst_cost = temp_cost;
            }
        }
    }

    if (found_equal) {
        second_mst_cost = mst_cost;
    } else if (second_mst_cost == INT_MAX) {
        second_mst_cost = mst_cost; // No second MST found, assign MST cost
    }

    cout << mst_cost << " " << second_mst_cost << endl;
    return 0;
}
