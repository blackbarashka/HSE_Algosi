#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <chrono>
//Musaev Umakhan.
using namespace std;

typedef uint64_t ull;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int main() {
    int n, m;
    cin >> n;
    vector<int> a(n);
    vector<int> all_elements;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        all_elements.push_back(a[i]);
    }
    cin >> m;
    vector<int> b(m);
    for (int i = 0; i < m; i++) {
        cin >> b[i];
        all_elements.push_back(b[i]);
    }

    // Сопоставляем элементы с индексами
    sort(all_elements.begin(), all_elements.end());
    all_elements.erase(unique(all_elements.begin(), all_elements.end()), all_elements.end());
    unordered_map<int, int> element_to_index;
    for (size_t i = 0; i < all_elements.size(); i++) {
        element_to_index[all_elements[i]] = static_cast<int>(i);
    }
    int total_unique = static_cast<int>(all_elements.size());

    // Генерируем случайные хеши для каждого уникального элемента
    vector<ull> h1(total_unique), h2(total_unique);
    for (int i = 0; i < total_unique; i++) {
        h1[i] = rng();
        h2[i] = rng();
    }

    // Преобразуем массивы в индексы и вычисляем префиксные суммы хешей
    vector<int> a_mapped(n);
    vector<ull> a_prefix_hash1(n + 1, 0), a_prefix_hash2(n + 1, 0);
    for (int i = 0; i < n; i++) {
        a_mapped[i] = element_to_index[a[i]];
        a_prefix_hash1[i + 1] = a_prefix_hash1[i] + h1[a_mapped[i]];
        a_prefix_hash2[i + 1] = a_prefix_hash2[i] + h2[a_mapped[i]];
    }

    vector<int> b_mapped(m);
    vector<ull> b_prefix_hash1(m + 1, 0), b_prefix_hash2(m + 1, 0);
    for (int i = 0; i < m; i++) {
        b_mapped[i] = element_to_index[b[i]];
        b_prefix_hash1[i + 1] = b_prefix_hash1[i] + h1[b_mapped[i]];
        b_prefix_hash2[i + 1] = b_prefix_hash2[i] + h2[b_mapped[i]];
    }

    int left = 1, right = min(n, m);
    int ans_k = 0, ans_i = -1, ans_j = -1;

    while (left <= right) {
        int k = (left + right) / 2;

        unordered_map<ull, int> hash_map;

        // Обрабатываем массив a
        for (int i = 0; i <= n - k; i++) {
            ull window_hash1 = a_prefix_hash1[i + k] - a_prefix_hash1[i];
            ull window_hash2 = a_prefix_hash2[i + k] - a_prefix_hash2[i];
            ull combined_hash = window_hash1 * 1000000007ULL + window_hash2;
            hash_map[combined_hash] = i;
        }

        bool found = false;
        int temp_j = -1, temp_i = -1;

        // Обрабатываем массив b
        for (int j = 0; j <= m - k; j++) {
            ull window_hash1 = b_prefix_hash1[j + k] - b_prefix_hash1[j];

            ull window_hash2 = b_prefix_hash2[j + k] - b_prefix_hash2[j];
            ull combined_hash = window_hash1 * 1000000007ULL + window_hash2;
            if (hash_map.count(combined_hash)) {
                temp_i = hash_map[combined_hash];
                temp_j = j;
                found = true;
                break;
            }
        }

        if (found) {
            ans_k = k;
            ans_i = temp_i;
            ans_j = temp_j;
            left = k + 1; // Пробуем найти большее k
        } else {
            right = k - 1; // Пробуем меньшее k
        }
    }

    if (ans_k > 0) {
        // Вывод в 1-based индексации
        cout << ans_k << " " << ans_i + 1 << " " << ans_j + 1 << endl;
    } else {
        cout << "0 0 0" << endl;
    }

    return 0;
}
