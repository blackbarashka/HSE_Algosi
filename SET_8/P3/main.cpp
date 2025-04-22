#include <iostream>
#include <vector>
#include <string>

// Вычисление префикс-функции (лучшая практика для KMP)
std::vector<int> computePrefixFunction(const std::string& P) {
    int M = P.size();
    std::vector<int> prefix(M, 0);
    for (int i = 1, k = 0; i < M; ++i) {
        while (k > 0 && P[k] != P[i])
            k = prefix[k - 1];
        if (P[k] == P[i])
            ++k;
        prefix[i] = k;
    }
    return prefix;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string P, T;
    std::cin >> P >> T;
    int M = P.size();
    int N = T.size();


    std::vector<int> prefix = computePrefixFunction(P);
    std::vector<int> results;

    for (int i = 0, matched = 0; i < N; ++i) {
        while (matched > 0 && P[matched] != T[i])
            matched = prefix[matched - 1];
        if (P[matched] == T[i])
            ++matched;
        if (matched == M) { // найдено вхождение
            results.push_back(i - M + 1);
            matched = prefix[matched - 1];
        }
    }

    std::cout << results.size() << "\n";
    for (int pos : results) {
        std::cout << pos << "\n";
    }
    return 0;
}
