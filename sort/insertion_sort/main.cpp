#include <iostream>
#include <vector>

int main() {
    std::cout << "Enter the lenght of the vector:" << '\n';
    int n;
    std::cin >> n;

    std::cout << "Enter the value of the vector elements:" << '\n';
    std::vector<int> vec(n);
    for (int i = 0 ; i < n; i++) {
        std::cin >> vec[i];
    }

    //Insertion Sort.
    for (int i = 0; i < n; i++) {
        int j = i-1;
        while (j >= 0 && vec[j] > vec[j+1]) {
            std::swap(vec[j], vec[j+1]);
            j--;
        }
    }

    for ( auto c : vec) {
        std::cout << c << " ";
    }
}
