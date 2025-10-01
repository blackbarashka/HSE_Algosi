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

    //Bubble sort.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n-1; j++) {
            if (vec[j] > vec[j+1]) {
                std::swap(vec[j+1], vec[j]);
            }
        }
    }

    for ( auto c : vec) {
        std::cout << c << " ";
    }
}
