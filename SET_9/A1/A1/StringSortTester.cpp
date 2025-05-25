#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <fstream>
#include "StringGenerator.cpp"

const std::string StringGenerator::ALLOWED_CHARS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";

// Функция для сохранения массива в txt-файл
void saveArrayToFile(const std::vector<std::string>& arr, const std::string& fileName) {
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        std::cerr << "Couldn`t open file: " << fileName << std::endl;
        return;
    }

    for (const auto& str : arr) {
        outFile << str << "\n";
    }
    outFile.close();

    std::cout << "Save in file: " << fileName << std::endl;
}


class StringSortTester {
private:
    // Counter for character-by-character comparisons
    static size_t characterComparisons;

    // Number of runs for averaging
    static const int TEST_RUNS = 5;

    static bool defaultCompare(const std::string& a, const std::string& b) {
        size_t minLen = std::min(a.size(), b.size());
        for (size_t i = 0; i < minLen; i++) {
            ++characterComparisons;
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        ++characterComparisons; // compare lengths
        return a.size() < b.size();
    }

    // Merge procedure for standard MergeSort
    static void merge(std::vector<std::string>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<std::string> L(n1), R(n2);
        for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
        for (int i = 0; i < n2; ++i) R[i] = arr[mid + 1 + i];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (defaultCompare(L[i], R[j])) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    // MergeSort algorithm
    static void mergeSortImpl(std::vector<std::string>& arr, int left, int right) {
        if (left < right) {
            int mid = (left + right) / 2;
            mergeSortImpl(arr, left, mid);
            mergeSortImpl(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

    // QuickSort algorithm
    static void quickSortImpl(std::vector<std::string>& arr, int left, int right) {
        if (left >= right) return;

        int pivotIndex = left + (right - left) / 2;
        const std::string& pivot = arr[pivotIndex];

        int i = left, j = right;
        while (i <= j) {
            while (defaultCompare(arr[i], pivot)) i++;
            while (defaultCompare(pivot, arr[j])) j--;
            if (i <= j) {
                std::swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
        if (left < j) quickSortImpl(arr, left, j);
        if (i < right) quickSortImpl(arr, i, right);
    }

    static void ternaryQuickSortImpl(std::vector<std::string>& arr, int left, int right, int depth) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        char pivotChar = charAt(arr[mid], depth);

        int lt = left, gt = right, i = left;
        while (i <= gt) {
            char currentChar = charAt(arr[i], depth);
            ++characterComparisons;
            if (currentChar < pivotChar) {
                std::swap(arr[lt++], arr[i++]);
            } else if (currentChar > pivotChar) {
                std::swap(arr[i], arr[gt--]);
            } else {
                i++;
            }
        }
        ternaryQuickSortImpl(arr, left, lt - 1, depth);
        if (pivotChar != '\0') {
            ternaryQuickSortImpl(arr, lt, gt, depth + 1);
        }
        ternaryQuickSortImpl(arr, gt + 1, right, depth);
    }

    static char charAt(const std::string& s, int d) {
        if (d >= (int)s.size()) return '\0';
        return s[d];
    }

    static int lcp(const std::string& a, const std::string& b) {
        int minLen = (int)std::min(a.size(), b.size());
        int i = 0;
        for (; i < minLen; i++) {
            ++characterComparisons;
            if (a[i] != b[i]) break;
        }
        return i;
    }

    static void mergeWithLcp(std::vector<std::string>& arr, int left, int mid, int right) {
        std::vector<std::string> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            int prefix = lcp(arr[i], arr[j]);
            if (prefix == (int)arr[i].size() || prefix == (int)arr[j].size()) {
                // one string is a prefix of the other
                temp[k++] = (arr[i].size() <= arr[j].size()) ? arr[i++] : arr[j++];
            } else if (arr[i][prefix] < arr[j][prefix]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        for (int idx = 0; idx < k; idx++) {
            arr[left + idx] = temp[idx];
        }
    }

    static void stringMergeSortLcpImpl(std::vector<std::string>& arr, int left, int right) {
        if (left >= right) return;
        int mid = (left + right) / 2;
        stringMergeSortLcpImpl(arr, left, mid);
        stringMergeSortLcpImpl(arr, mid + 1, right);
        mergeWithLcp(arr, left, mid, right);
    }

    static const int R = 256;
    static void msdRadixSortImpl(std::vector<std::string>& arr, int left, int right, int depth) {
        if (left >= right) return;
        if (right - left <= 10) {
            insertionSortFrom(arr, left, right, depth);
            return;
        }
        std::vector<int> count(R + 2, 0);
        std::vector<std::string> aux(right - left + 1);

        for (int i = left; i <= right; i++) {
            int c = charAtForRadix(arr[i], depth);
            ++characterComparisons;
            count[c + 2]++;
        }
        for (int r = 0; r < (int)count.size() - 1; r++) {
            count[r + 1] += count[r];
        }
        for (int i = left; i <= right; i++) {
            int c = charAtForRadix(arr[i], depth);
            aux[count[c + 1]++] = arr[i];
        }
        for (int i = left; i <= right; i++) {
            arr[i] = aux[i - left];
        }
        for (int r = 0; r < R; r++) {
            msdRadixSortImpl(arr, left + count[r], left + count[r + 1] - 1, depth + 1);
        }
    }

    static void msdRadixSortWithQuickImpl(std::vector<std::string>& arr, int left, int right, int depth) {
        if (left >= right) return;

        // Switch threshold
        static const int SWITCH_THRESHOLD = 74;
        if (right - left + 1 <= SWITCH_THRESHOLD) {
            ternaryQuickSortImpl(arr, left, right, depth);
            return;
        }

        std::vector<int> count(R + 2, 0);
        std::vector<std::string> aux(right - left + 1);

        for (int i = left; i <= right; i++) {
            int c = charAtForRadix(arr[i], depth);
            ++characterComparisons;
            count[c + 2]++;
        }
        for (int r = 0; r < (int)count.size() - 1; r++) {
            count[r + 1] += count[r];
        }
        for (int i = left; i <= right; i++) {
            int c = charAtForRadix(arr[i], depth);
            aux[count[c + 1]++] = arr[i];
        }
        for (int i = left; i <= right; i++) {
            arr[i] = aux[i - left];
        }
        for (int r = 0; r < R; r++) {
            msdRadixSortWithQuickImpl(arr, left + count[r], left + count[r + 1] - 1, depth + 1);
        }
    }

    static int charAtForRadix(const std::string& s, int d) {
        if (d < (int)s.size()) return (unsigned char)s[d];
        return -1;
    }

    static void insertionSortFrom(std::vector<std::string>& arr, int left, int right, int depth) {
        for (int i = left + 1; i <= right; i++) {
            for (int j = i; j > left; j--) {
                if (compareFrom(arr[j], arr[j - 1], depth) < 0) {
                    std::swap(arr[j], arr[j - 1]);
                } else {
                    break;
                }
            }
        }
    }

    static int compareFrom(const std::string& a, const std::string& b, int depth) {
        int lenA = (int)a.size(), lenB = (int)b.size();
        for (int i = depth; i < lenA && i < lenB; i++) {
            ++characterComparisons;
            if (a[i] < b[i]) return -1;
            if (a[i] > b[i]) return 1;
        }
        ++characterComparisons;
        return (lenA - depth) - (lenB - depth);
    }

public:
    struct TestResult {
        double avgTimeMs;
        size_t avgComparisons;
    };

    template<typename SortFunc>
    static TestResult runTest(const std::vector<std::string>& arr, SortFunc sortFunc) {
        std::vector<double> times;
        std::vector<size_t> comps;

        for (int attempt = 0; attempt < TEST_RUNS; attempt++) {
            std::vector<std::string> localCopy = arr;
            characterComparisons = 0;

            auto start = std::chrono::high_resolution_clock::now();
            sortFunc(localCopy);
            auto end = std::chrono::high_resolution_clock::now();

            double ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
            times.push_back(ms);
            comps.push_back(characterComparisons);
        }

        double totalTime = std::accumulate(times.begin(), times.end(), 0.0);
        size_t totalComp = std::accumulate(comps.begin(), comps.end(), (size_t)0);

        TestResult tr;
        tr.avgTimeMs = totalTime / TEST_RUNS;
        tr.avgComparisons = totalComp / TEST_RUNS;
        return tr;
    }

    static TestResult testQuickSort(const std::vector<std::string>& arr) {
        return runTest(arr, [](std::vector<std::string>& data){
            quickSortImpl(data, 0, (int)data.size() - 1);
        });
    }

    static TestResult testMergeSort(const std::vector<std::string>& arr) {
        return runTest(arr, [](std::vector<std::string>& data){
            mergeSortImpl(data, 0, (int)data.size() - 1);
        });
    }

    static TestResult testTernaryStringQuick(const std::vector<std::string>& arr) {
        return runTest(arr, [](std::vector<std::string>& data){
            ternaryQuickSortImpl(data, 0, (int)data.size() - 1, 0);
        });
    }

    static TestResult testStringMergeSortLCP(const std::vector<std::string>& arr) {
        return runTest(arr, [](std::vector<std::string>& data){
            stringMergeSortLcpImpl(data, 0, (int)data.size() - 1);
        });
    }

    static TestResult testMsdRadixSort(const std::vector<std::string>& arr) {
        return runTest(arr, [](std::vector<std::string>& data){
            msdRadixSortImpl(data, 0, (int)data.size() - 1, 0);
        });
    }

    static TestResult testMsdRadixSortWithQuick(const std::vector<std::string>& arr) {
        return runTest(arr, [](std::vector<std::string>& data){
            msdRadixSortWithQuickImpl(data, 0, (int)data.size() - 1, 0);
        });
    }

    static void saveResultsCsv(
        const std::string& fileName,
        const std::vector<std::pair<int, std::vector<TestResult>>>& allResults,
        const std::vector<std::string>& algorithmNames
    ) {
        std::ofstream fout(fileName);
        if (!fout.is_open()) {
            std::cerr << "Failed to open file: " << fileName << std::endl;
            return;
        }


        fout << "ArraySize";
        for (auto& alg : algorithmNames) {
            fout << "," << alg << "_TimeMs," << alg << "_Comparisons";
        }
        fout << "\n";

        // Data rows
        for (auto& line : allResults) {
            fout << line.first;
            for (auto& result : line.second) {
                fout << "," << result.avgTimeMs << "," << result.avgComparisons;
            }
            fout << "\n";
        }
        fout.close();
    }
};

size_t StringSortTester::characterComparisons = 0;
const int StringSortTester::TEST_RUNS;

int main() {
    std::cout << "Performance analysis of string sorting algorithms" << std::endl;

    // Создаем генератор и строим 4 набора строк размером до 3000
    StringGenerator generator(42);
    auto randomFull = generator.generateRandomArray(3000);
    auto reverseFull = generator.generateReverseSortedArray(3000);
    auto nearlyFull = generator.generateNearlySortedArray(3000);
    auto prefixFull = generator.generatePrefixArray(3000, "ABC");

    // Сохраняем эти сгенерированные данные в txt-файлы:
    saveArrayToFile(randomFull,  "random_data.txt");
    saveArrayToFile(reverseFull, "reverse_data.txt");
    saveArrayToFile(nearlyFull,  "nearly_data.txt");
    saveArrayToFile(prefixFull,  "prefix_data.txt");

    std::vector<std::string> algNames = {
        "QuickSort",
        "MergeSort",
        "STRINGQUICKSORT",
        "STRINGMERGESORT",
        "MSDRadix",
        "MSDRadix_Quick"
    };

    // Размеры подмассивов
    std::vector<int> sizes = {100, 200, 300, 500, 750, 1000, 1500, 2000, 2250, 2500, 2750, 3000};

    // Подготовка контейнеров для результатов
    std::vector<std::pair<int, std::vector<StringSortTester::TestResult>>> randomResults;
    std::vector<std::pair<int, std::vector<StringSortTester::TestResult>>> reverseResults;
    std::vector<std::pair<int, std::vector<StringSortTester::TestResult>>> nearlyResults;
    std::vector<std::pair<int, std::vector<StringSortTester::TestResult>>> prefixResults;

    // Запускаем тестирование для каждого выбранного размера
    for (auto sz : sizes) {
        auto randomArr = generator.getSubArray(randomFull, sz);
        auto reverseArr = generator.getSubArray(reverseFull, sz);
        auto nearlyArr = generator.getSubArray(nearlyFull, sz);
        auto prefixArr = generator.getSubArray(prefixFull, sz);

        // Тестируем randomArray
        std::vector<StringSortTester::TestResult> tmpRandom;
        tmpRandom.push_back(StringSortTester::testQuickSort(randomArr));
        tmpRandom.push_back(StringSortTester::testMergeSort(randomArr));
        tmpRandom.push_back(StringSortTester::testTernaryStringQuick(randomArr));
        tmpRandom.push_back(StringSortTester::testStringMergeSortLCP(randomArr));
        tmpRandom.push_back(StringSortTester::testMsdRadixSort(randomArr));
        tmpRandom.push_back(StringSortTester::testMsdRadixSortWithQuick(randomArr));
        randomResults.push_back({sz, tmpRandom});

        // Тестируем reverseArray
        std::vector<StringSortTester::TestResult> tmpReverse;
        tmpReverse.push_back(StringSortTester::testQuickSort(reverseArr));
        tmpReverse.push_back(StringSortTester::testMergeSort(reverseArr));
        tmpReverse.push_back(StringSortTester::testTernaryStringQuick(reverseArr));
        tmpReverse.push_back(StringSortTester::testStringMergeSortLCP(reverseArr));
        tmpReverse.push_back(StringSortTester::testMsdRadixSort(reverseArr));
        tmpReverse.push_back(StringSortTester::testMsdRadixSortWithQuick(reverseArr));
        reverseResults.push_back({sz, tmpReverse});

        // Тестируем nearlyArray
        std::vector<StringSortTester::TestResult> tmpNearly;
        tmpNearly.push_back(StringSortTester::testQuickSort(nearlyArr));
        tmpNearly.push_back(StringSortTester::testMergeSort(nearlyArr));
        tmpNearly.push_back(StringSortTester::testTernaryStringQuick(nearlyArr));
        tmpNearly.push_back(StringSortTester::testStringMergeSortLCP(nearlyArr));
        tmpNearly.push_back(StringSortTester::testMsdRadixSort(nearlyArr));
        tmpNearly.push_back(StringSortTester::testMsdRadixSortWithQuick(nearlyArr));
        nearlyResults.push_back({sz, tmpNearly});

        // Тестируем prefixArray
        std::vector<StringSortTester::TestResult> tmpPrefix;
        tmpPrefix.push_back(StringSortTester::testQuickSort(prefixArr));
        tmpPrefix.push_back(StringSortTester::testMergeSort(prefixArr));
        tmpPrefix.push_back(StringSortTester::testTernaryStringQuick(prefixArr));
        tmpPrefix.push_back(StringSortTester::testStringMergeSortLCP(prefixArr));
        tmpPrefix.push_back(StringSortTester::testMsdRadixSort(prefixArr));
        tmpPrefix.push_back(StringSortTester::testMsdRadixSortWithQuick(prefixArr));
        prefixResults.push_back({sz, tmpPrefix});
    }

    // Сохраняем результаты в CSV для анализа
    StringSortTester::saveResultsCsv("random_results.csv",   randomResults,  algNames);
    StringSortTester::saveResultsCsv("reverse_results.csv",  reverseResults, algNames);
    StringSortTester::saveResultsCsv("nearly_results.csv",   nearlyResults,  algNames);
    StringSortTester::saveResultsCsv("prefix_results.csv",   prefixResults,  algNames);

    std::cout << std::endl << "Done. Results have been saved to CSV and TXT files." << std::endl;
    return 0;
}
