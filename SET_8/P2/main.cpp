#include <iostream>
#include <vector>
// Rезультаты вычисления префикс-функции записываем в вектор
std::vector<size_t> prefixFun;

void computePrefixFun(const std::string& str) {
    size_t n = str.length();
    prefixFun.assign(n, 0);

    for (size_t i = 1; i < n; ++i) {
        size_t k = prefixFun[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = prefixFun[k - 1];
        }
        if (str[i] == str[k]) {
            ++k;
        }
        prefixFun[i] = k;
    }
}

int main() {
    std::string str;
    std::cin >> str;
    computePrefixFun(str);

    size_t len = str.length();
    size_t minimal_length_s = len - prefixFun[len - 1];
    std::cout << minimal_length_s << std::endl;

    return 0;
}
