#include <iostream>
#include <vector>
void merge(std::vector<int>& vec, int left, int mid, int right);

void quicksort(std::vector<int>& vec, int left, int right) {
    if (right <= left) return;
    int pivot = vec[(left + right)/2];
    int i = left;
    int j = right;
    while (i <= j) {
        while(vec[i] < pivot) i++;
        while(vec[j] > pivot) j--;
        if (i <= j) {
            std::swap(vec[i], vec[j]);
        }

        i++; j--;
    }
    quicksort(vec,left,j);
    quicksort(vec,i, right);

}



int main() {
    std::cout << "Enter the length of the vector:" << '\n';
    int n;
    std::cin >> n;


    std::cout << "Enter the value of the vector elements:" << '\n';
    std::vector<int> vec(n);
    for (int i = 0 ; i < n; i++) {
        std::cin >> vec[i];
    }

    quicksort(vec,0, vec.size()-1);

    for ( auto c : vec) {
        std::cout << c << " ";
    }
}
