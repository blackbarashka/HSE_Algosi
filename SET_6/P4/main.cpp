//Musaev Umakhan.
#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const long long INF = 1e18;

int main() {
    int n, m;
    cin >> n >> m;

    // Инициализация матрицы расстояний
    vector<vector<long long>> dist(n, vector<long long>(n, INF));

    // Вес ребра из вершины в себя равен 0
    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
    }

    // Чтение рёбер графа
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w); // Учитываем возможность нескольких дуг между вершинами
    }

    // Алгоритм Флойда-Уоршелла
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Вывод всех пар кратчайших путей
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (u != v) {
                if (dist[u][v] == INF) {
                    cout << u << " " << v << " -1" << endl; // Если пути нет
                } else {
                    cout << u << " " << v << " " << dist[u][v] << endl; // Вес пути
                }
            }
        }
    }

    return 0;
}
