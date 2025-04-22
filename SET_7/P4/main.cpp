//Musaev Umakhan.
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct Cell {
    int x, y;
    Cell(int x = 0, int y = 0) : x(x), y(y) {}
};

vector<Cell> black, white;
map<pair<int, int>, int> white_index;
vector<vector<int>> adj;
vector<int> visited;
vector<int> matching;

bool dfs(int u, int visit_time) {
    if (visited[u] == visit_time) return false;
    visited[u] = visit_time;
    for (int v : adj[u]) {
        if (matching[v] == -1 || dfs(matching[v], visit_time)) {
            matching[v] = u;
            return true;
        }
    }
    return false;
}

int main() {
    int n, m, a, b;
    cin >> n >> m >> a >> b;

    vector<string> grid(n);
    int S = 0;
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '*') {
                S++;
                if ((i + j) % 2 == 0) {
                    black.emplace_back(i, j);
                } else {
                    white.emplace_back(i, j);
                }
            }
        }
    }

    if (S == 0) {
        cout << 0 << endl;
        return 0;
    }

    for (int i = 0; i < white.size(); ++i) {
        white_index[{white[i].x, white[i].y}] = i;
    }

    adj.resize(black.size());
    for (int i = 0; i < black.size(); ++i) {
        int x = black[i].x, y = black[i].y;
        vector<pair<int, int>> neighbors = {{x-1, y}, {x+1, y}, {x, y-1}, {x, y+1}};
        for (auto& p : neighbors) {
            int nx = p.first, ny = p.second;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '*') {
                auto it = white_index.find({nx, ny});
                if (it != white_index.end()) {
                    adj[i].push_back(it->second);
                }
            }
        }
    }

    int max_d = 0;
    matching.assign(white.size(), -1);
    visited.assign(black.size(), -1);

    for (int u = 0; u < black.size(); ++u) {
        if (dfs(u, u)) {
            max_d++;
        }
    }

    int cost_domino = max_d * a + (S - 2 * max_d) * b;
    int cost_single = S * b;

    cout << min(cost_domino, cost_single) << endl;

    return 0;
}