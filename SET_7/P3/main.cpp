#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring> // для memset

using namespace std;

const int MAXN = 1e5 + 5; // максимальное число вершин
const long long INF = 1e18;

struct Edge {
    int to, rev;
    long long cap, flow;
};

vector<Edge> adj[MAXN];
int level[MAXN];
int ptr[MAXN];
int n; // число вершин

void addEdge(int a, int b, long long cap) {
    Edge e1 = {b, (int)adj[b].size(), cap, 0};
    Edge e2 = {a, (int)adj[a].size(), 0, 0}; // если граф ориентированный
    adj[a].push_back(e1);
    adj[b].push_back(e2);
}

bool bfs(int s, int t) {
    queue<int> q;
    memset(level, -1, n * sizeof(level[0]));
    level[s] = 0;
    q.push(s);
    while (!q.empty() && level[t] == -1) {
        int v = q.front();
        q.pop();
        for (Edge &e : adj[v]) {
            if (level[e.to] == -1 && e.cap - e.flow > 0) {
                level[e.to] = level[v] + 1;
                q.push(e.to);
            }
        }
    }
    return level[t] != -1;
}

long long dfs(int v, int t, long long pushed) {
    if (pushed == 0)
        return 0;
    if (v == t)
        return pushed;
    for (int &cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
        Edge &e = adj[v][cid];
        if (level[e.to] != level[v] + 1 || e.cap - e.flow <= 0)
            continue;
        long long tr = dfs(e.to, t, min(pushed, e.cap - e.flow));
        if (tr == 0)
            continue;
        e.flow += tr;
        adj[e.to][e.rev].flow -= tr;
        return tr;
    }
    return 0;
}

long long dinic(int s, int t) {
    long long maxFlow = 0;
    while (bfs(s, t)) {
        memset(ptr, 0, n * sizeof(ptr[0]));
        while (long long flow = dfs(s, t, INF)) {
            maxFlow += flow;
        }
    }
    return maxFlow;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> n >> m;
    // Считываем граф
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cap;
        cin >> u >> v >> cap;
        --u, --v; // если вершины нумеруются с 1
        addEdge(u, v, cap);
        // Если граф неориентированный, добавляем ребро в обратную сторону
        // addEdge(v, u, cap);
    }

    int s, t;
    cin >> s >> t;
    --s, --t;

    long long max_flow = dinic(s, t);
    cout << max_flow << '\n';

    return 0;
}
