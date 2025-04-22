//Musaev Umahan.
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
const int MAXN = 251;

vector<int> adj[MAXN];
int pair_u[MAXN], pair_v[MAXN]; // Массивы для хранения паросочетания.
int dist[MAXN];
int n, m;
bool dfs(int u) {
    for (int v : adj[u]) {
        if (pair_v[v] == 0 || (dist[pair_v[v]] == dist[u] + 1 && dfs(pair_v[v]))) {
            pair_u[u] = v;
            pair_v[v] = u;
            return true;
        }
    }
    dist[u] = -1;
    return false;
}
bool bfs() {
    queue<int> Q;
    for (int u = 1; u <= n; ++u) {
        if (pair_u[u] == 0) {
            dist[u] = 0;
            Q.push(u);
        } else {
            dist[u] = -1;
        }
    }
    bool found = false;
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        for (int v : adj[u]) {
            if (pair_v[v] == 0) {
                found = true;
            } else if (dist[pair_v[v]] == -1) {
                dist[pair_v[v]] = dist[u] + 1;
                Q.push(pair_v[v]);
            }
        }
    }
    return found;
}



int hopcroft_karp() {
    fill(pair_u, pair_u + MAXN, 0);
    fill(pair_v, pair_v + MAXN, 0);
    int matching = 0;
    while (bfs()) {
        for (int u = 1; u <= n; ++u) {
            if (pair_u[u] == 0 && dfs(u)) {
                matching++;
            }
        }
    }
    return matching;
}

int main() {
    cin >> n >> m;
    string line;
    getline(cin, line); // Считать остаток строки после чтения m.
    for (int u = 1; u <= n; ++u) {
        getline(cin, line);
        int pos = 0;
        int len = line.length();
        while (pos < len) {
            while (pos < len && line[pos] == ' ') pos++;
            if (pos >= len) break;
            int v = 0;
            while (pos < len && line[pos] >= '0' && line[pos] <= '9') {
                v = v * 10 + (line[pos] - '0');
                pos++;
            }
            if (v == 0) break;
            adj[u].push_back(v);
        }
    }

    int matching = hopcroft_karp();
    cout << matching << endl;
    for (int u = 1; u <= n; ++u) {
        if (pair_u[u] != 0) {
            cout << u << " " << pair_u[u] << endl;
        }
    }
    return 0;
}
