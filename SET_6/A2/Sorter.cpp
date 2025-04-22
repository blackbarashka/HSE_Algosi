// SortTester.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "ArrayGenerator.cpp"

class SortTester {
public:
    // Стандартный Merge Sort.
    void mergeSort(std::vector<int> &arr, int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
            merge(arr, l, m, r);
        }
    }

    // Гибридный Merge+Insertion Sort.
    void hybridSort(std::vector<int> &arr, int l, int r, int threshold) {
        if (r - l + 1 <= threshold) {
            insertionSort(arr, l, r);
        } else {
            int m = l + (r - l) / 2;
            hybridSort(arr, l, m, threshold);
            hybridSort(arr, m + 1, r, threshold);
            merge(arr, l, m, r);
        }
    }

    // Тестирование сортировок на различных массивах и запись результатов в файл.
    void testSorts(const std::vector<std::vector<int>> &testArrays, const std::vector<std::string> &arrayTypes, const std::vector<int> &thresholds, const std::vector<int> &sizes) {
        std::ofstream outfile("results.csv");


        for(size_t i = 0; i < testArrays.size(); ++i) {
            const auto &arr = testArrays[i];
            const std::string &type = arrayTypes[i];
            int size = sizes[i / arrayTypesPerSize]; // Каждый размер повторяется arrayTypesPerSize раз.

            // Форматирование размера с ведущими нулями (например, "0000")
            std::ostringstream sizeStream;
            sizeStream << std::setw(5) << std::setfill('0') << size;
            std::string sizeStr = sizeStream.str();

            // Стандартный Merge Sort с параметром 0.
            std::vector<int> arrMerge = arr;
            auto start = std::chrono::high_resolution_clock::now();
            mergeSort(arrMerge, 0, arrMerge.size() - 1);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microsec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            outfile << sizeStr << ";\"" << type << "\";0;" << microsec << "\n";

            // Гибридный сорт для разных порогов.
            for(auto threshold : thresholds) {
                std::vector<int> arrHybrid = arr;
                auto startHy = std::chrono::high_resolution_clock::now();
                hybridSort(arrHybrid, 0, arrHybrid.size() - 1, threshold);
                auto elapsedHy = std::chrono::high_resolution_clock::now() - startHy;
                long long microsecHy = std::chrono::duration_cast<std::chrono::microseconds>(elapsedHy).count();
                outfile << sizeStr << ";\"" << type << "\";" << threshold << ";" << microsecHy << "\n";
            }
        }

        outfile.close();
    }

private:
    int arrayTypesPerSize = 3; // Количество типов массивов на каждый размер.

    // Функция слияния для Merge Sort.
    void merge(std::vector<int> &arr, int l, int m, int r) {

        int n1 = m - l + 1;
        int n2 = r - m;
        std::vector<int> L(n1);
        std::vector<int> R(n2);
        for(int i = 0; i < n1; ++i)
            L[i] = arr[l + i];
        for(int j = 0; j < n2; ++j)
            R[j] = arr[m + 1 + j];
        int i = 0, j = 0, k = l;
        while(i < n1 && j < n2) {
            if(L[i] <= R[j]) {
                arr[k++] = L[i++];
            }
            else {
                arr[k++] = R[j++];
            }
        }
        while(i < n1) arr[k++] = L[i++];
        while(j < n2) arr[k++] = R[j++];
    }

    // Вставки Сортировка для гибридного алгоритма.
    void insertionSort(std::vector<int> &arr, int l, int r) {
        for(int i = l + 1; i <= r; ++i) {
            int key = arr[i];
            int j = i - 1;
            while(j >= l && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
};

int main() {
    ArrayGenerator generator;
    SortTester tester;

    // Параметры генерации массива.
    std::vector<int> sizes;
    for(int size = 500; size <= 10000; size += 100) {
        sizes.push_back(size);
    }

    // Виды массивов.
    std::vector<std::vector<int>> testArrays;
    std::vector<std::string> arrayTypes;
    std::vector<int> relevantSizes;

    // Генерация массивов.
    for(auto size : sizes) {
        // Случайные массивы
        testArrays.push_back(generator.generateRandomArray(size));
        arrayTypes.push_back("random");
        relevantSizes.push_back(size);

        // Перемешанные массивы
        testArrays.push_back(generator.generateShuffledArray(size, 0.1));
        arrayTypes.push_back("shuffled");
        relevantSizes.push_back(size);

        // Частично отсортированные массивы
        testArrays.push_back(generator.generatePartiallySortedArray(size, 0.9));
        arrayTypes.push_back("partially_sorted");
        relevantSizes.push_back(size);
    }

    // Пороговые значения для гибридного алгоритма.
    std::vector<int> thresholds = {5, 10, 20, 30, 50};

    // Тестирование сортировок и запись результатов.
    tester.testSorts(testArrays, arrayTypes, thresholds, sizes);

    return 0;
}
