#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    int k;
    cin >> k;

    vector<int> codes(k);
    for (int i = 0; i < k; i++) {
        cin >> codes[i];
    }

    vector<string> dictionary(128);
    for (int i = 0; i < 128; i++) {
        dictionary[i] = string(1, char(i));
    }


    if (codes.empty()) {

        cout << "" << "\n";
        return 0;
    }

    // Декодируем первый символ
    int oldCode = codes[0];

    string decoded = dictionary[oldCode];
    cout << decoded;

    int dictSize = 128;  // Текущий размер словаря

    for (int i = 1; i < k; i++) {
        int newCode = codes[i];
        string entry;


        if (newCode < dictSize) {
            entry = dictionary[newCode];
        } else {

            entry = dictionary[oldCode] + dictionary[oldCode][0];
        }


        cout << entry;


        if (dictSize <= 100000) {

            dictionary.push_back(dictionary[oldCode] + entry[0]);
            dictSize++;
        }

        oldCode = newCode;
    }

    cout << "\n";
    return 0;
}