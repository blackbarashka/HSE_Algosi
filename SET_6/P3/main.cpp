//Musaev Umakhan.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class UnionFind {
public:
    UnionFind(int n) : par(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            par[i] = i;
        }
    }

    int find(int x) {
        if (par[x] != x) {
            par[x] = find(par[x]); // Path compression
        }
        return par[x];
    }

    void unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                par[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                par[rootX] = rootY;
            } else {
                par[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }

private:
    vector<int> par;
    vector<int> rank;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u - 1, v - 1}; // использовать нумерацию от нуля
    }

    vector<string> operations(k);
    vector<bool> isAsk(k);
    vector<int> u(k), v(k);

    for (int i = 0; i < k; ++i) {
        string opType;
        cin >> opType >> u[i] >> v[i];
        operations[i] = opType;
        u[i]--;
        v[i]--;
        isAsk[i] = (opType == "ask");
    }

    UnionFind uf(n);
    vector<string> results;

    for (int i = k - 1; i >= 0; --i) {
        if (isAsk[i]) {
            if (uf.find(u[i]) == uf.find(v[i])) {
                results.push_back("YES");
            } else {
                results.push_back("NO");
            }
        } else {
            uf.unionSets(u[i], v[i]);
        }
    }

    reverse(results.begin(), results.end());
    for (const string& result : results) {
        cout << result << '\n';
    }

    return 0;
}
