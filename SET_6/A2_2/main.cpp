#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int from, to;
    double w;
};

// Восстанавливаем ориентированный граф:
// Если dist[i][j] "не выводится" через другую вершину,
// то считаем, что (i->j) — ребро с весом dist[i][j].
vector<Edge> RestoreGraph(const vector<vector<double>>& dist) {
    int n = (int)dist.size();
    vector<Edge> edges;
    edges.reserve(n*(n-1)); // в худшем случае

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i == j) continue;
            // Проверяем, не получается ли dist[i][j] = dist[i][k] + dist[k][j] для некоторого k
            bool isDirectEdge = true;
            for(int k=0; k<n; k++){
                if(k==i || k==j) continue;
                // Если нашли, что dist[i][j] совпадает с суммой, значит
                // ребро (i->j) можно не добавлять - оно выводится через k
                if(fabs(dist[i][j] - (dist[i][k] + dist[k][j])) < 1e-9) {
                    isDirectEdge = false;
                    break;
                }
            }
            if(isDirectEdge) {
                // Добавляем ребро
                Edge e{i, j, dist[i][j]};
                edges.push_back(e);
            }
        }
    }
    return edges;
}

int main(){
    // Пример: матрица кратчайших расстояний 4x4
    // Пусть dist[i][j] формально согласована с каким-то графом
    vector<vector<double>> dist = {
       {0, 2, 3, 5},
       {2, 0, 1, 4},
       {3, 1, 0, 3},
       {5, 4, 3, 0}
    };

    auto edges = RestoreGraph(dist);
    for(auto &e : edges){
        cout << e.from << " -> " << e.to << "  ves=" << e.w << "\n";
    }

}
