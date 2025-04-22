#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Edge {
    int to, rev;
    int cap;
};

const int MAXN = 100000 + 5;
vector<Edge> adj[MAXN];
int n, m, a, h;

// Добавляем ребро для НЕориентированного графа
// Обратите внимание, что и в прямом, и в обратном направлении ставим "cap = 1".
void add_edge(int u, int v) {
    // Ребро (u -> v)
    adj[u].push_back({v, (int)adj[v].size(), 1});
    // Ребро (v -> u)
    adj[v].push_back({u, (int)adj[u].size() - 1, 1});
}

// Поиск в ширину для нахождения увеличивающего пути в остаточной сети
bool bfs(int s, int t, vector<int>& parent, vector<int>& parent_edge) {
    vector<bool> visited(n + 1, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == t) {
            return true;
        }
        for (int i = 0; i < (int)adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (!visited[e.to] && e.cap > 0) {
                visited[e.to]     = true;
                parent[e.to]      = u;
                parent_edge[e.to] = i;
                q.push(e.to);
            }
        }
    }
    return false;
}

// Находим один путь из s в t (если он есть) и «прокачиваем» по нему 1 единицу потока
vector<int> find_path(int s, int t) {
    vector<int> parent(n + 1);
    vector<int> parent_edge(n + 1);

    bool found = bfs(s, t, parent, parent_edge);
    if (!found) {
        return {};
    }

    // Восстанавливаем путь и обновляем остаточные пропускные способности
    vector<int> path;
    int cur = t;
    while (cur != s) {
        int p   = parent[cur];
        int idx = parent_edge[cur];

        path.push_back(cur);

        // Уменьшаем пропускную способность прямого ребра
        adj[p][idx].cap -= 1;

        // Увеличиваем пропускную способность обратного ребра
        int rev_idx = adj[p][idx].rev;
        adj[cur][rev_idx].cap += 1;

        cur = p;
    }
    path.push_back(s);
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> a >> h;
    // Если начальная и конечная вершина совпадают — нет смысла искать пути
    if (a == h) {
        cout << "NO\n";
        return 0;
    }

    // Считываем рёбра
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        add_edge(x, y);
    }

    // Ищем первый путь
    vector<int> path1 = find_path(a, h);
    if (path1.empty()) {
        cout << "NO\n";
        return 0;
    }

    // Ищем второй путь
    vector<int> path2 = find_path(a, h);
    if (path2.empty()) {
        cout << "NO\n";
        return 0;
    }

    // Нашли два различных путя
    cout << "YES\n";
    for (int v : path1) {
        cout << v << ' ';
    }
    cout << '\n';
    for (int v : path2) {
        cout << v << ' ';
    }
    cout << '\n';

    return 0;
}