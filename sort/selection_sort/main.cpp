#include <iostream>
#include <vector>
void merge(std::vector<int>& vec, int left, int mid, int right);

void mergesort(std::vector<int>& vec, int left, int right) {
    if (left >= right) return;

    int mid  = left + (right - left)/2;

    mergesort(vec, left, mid);
    mergesort(vec, mid+1, right);
    merge(vec, left, mid, right);
}

void merge(std::vector<int>& vec, int left, int mid, int right) {
    std::vector<int> temp;
    int i = left;
    int j = mid + 1;
    while(i <= mid && j <= right) {
        if (vec[i] <= vec[j]) {
            temp.push_back(vec[i++]);
        }
        else {
            temp.push_back((vec[j++]));
        }
    }

    while(i <= mid) temp.push_back(vec[i++]);
    while(j <= right) temp.push_back(vec[j++]);

    for (int k = 0; k < temp.size(); k++) {
        vec[left + k] = temp[k];
    }
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

    mergesort(vec,0, vec.size()-1);

    for ( auto c : vec) {
        std::cout << c << " ";
    }
}
