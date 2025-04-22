//Musaev Umakhan.
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
typedef long long ll;
const ll INF = 1LL << 60;

struct Edge {
    int to, rev;
    ll cap, flow;
};

class MaxFlow {
public:
    int N;
    vector<vector<Edge>> G;
    vector<int> level;
    vector<int> iter;

    MaxFlow(int N) : N(N), G(N), level(N), iter(N) {}
    // Добавление ребра в граф.
    void add_edge(int from, int to, ll cap) {
        Edge a = {to, (int)G[to].size(), cap, 0};
        Edge b = {from, (int)G[from].size(), 0, 0}; // Обратное ребро с нулевой пропускной способностью.
        G[from].push_back(a);
        G[to].push_back(b);
    }

    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while(!que.empty()) {
            int v = que.front(); que.pop();
            for(const Edge &e : G[v]) {
                if(e.cap - e.flow > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    ll dfs(int v, int t, ll upTo) {
        if(v == t) return upTo;
        for(int &i = iter[v]; i < G[v].size(); ++i) {
            Edge &e = G[v][i];
            if(e.cap - e.flow > 0 && level[v] < level[e.to]) {
                ll d = dfs(e.to, t, min(upTo, e.cap - e.flow));
                if(d > 0) {
                    e.flow += d;
                    G[e.to][e.rev].flow -= d;
                    return d;
                }
            }
        }
        return 0;
    }

    ll max_flow(int s, int t) {
        ll flow = 0;
        while(true) {
            bfs(s);
            if(level[t] < 0) break;
            fill(iter.begin(), iter.end(), 0);
            while(ll f = dfs(s, t, INF)) {
                flow += f;
            }
        }
        return flow;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    MaxFlow mf(n);
    for(int i = 0; i < m; ++i) {
        int u, v;
        ll cap;
        cin >> u >> v >> cap;
        --u; --v; // Переводим вершины к нумерации с нуля.
        mf.add_edge(u, v, cap);
    }
    cout << mf.max_flow(0, n - 1) << endl;
    return 0;
}
