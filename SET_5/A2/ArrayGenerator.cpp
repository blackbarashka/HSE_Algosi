#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

class ArrayGenerator {
public:
    // Конструктор с инициализацией генератора случайных чисел.
    ArrayGenerator(int min_value = 0, int max_value = 6000)
        : min_val(min_value), max_val(max_value),
          rng(std::chrono::steady_clock::now().time_since_epoch().count()),
          dist(min_val, max_val) {}

    // Генерация случайного массива.
    std::vector<int> generateRandomArray(int size) {
        std::vector<int> arr(size);
        for(auto &elem : arr) {
            elem = dist(rng);
        }
        return arr;
    }

    // Генерация перемешанного массива с заданным процентом перестановок.
    std::vector<int> generateShuffledArray(int size, double shuffle_percentage = 0.1) {
        std::vector<int> arr = generateRandomArray(size);
        int num_swaps = static_cast<int>(size * shuffle_percentage);
        for(int i = 0; i < num_swaps; ++i) {
            int idx1 = dist(rng) % size;
            int idx2 = dist(rng) % size;
            std::swap(arr[idx1], arr[idx2]);
        }
        return arr;
    }

    // Генерация частично отсортированного массива с заданным процентом отсортированных элементов.
    std::vector<int> generatePartiallySortedArray(int size, double sorted_percentage = 0.9) {
        std::vector<int> arr = generateRandomArray(size);
        // Сортируем первые sorted_percentage элементов
        int sorted_size = static_cast<int>(size * sorted_percentage);
        std::sort(arr.begin(), arr.begin() + sorted_size);
        return arr;
    }

private:
    int min_val;
    int max_val;
    std::mt19937_64 rng;
    std::uniform_int_distribution<int> dist;
};
