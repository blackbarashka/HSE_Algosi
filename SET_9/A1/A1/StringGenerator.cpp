#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>


class StringGenerator {
private:
    static const std::string ALLOWED_CHARS; // 74 distinct characters
    std::mt19937 rng;
    std::uniform_int_distribution<int> lengthDist{10, 200};

    // Generate a single random string
    std::string generateSingleString() {
        int length = lengthDist(rng);
        std::uniform_int_distribution<size_t> charDist(0, ALLOWED_CHARS.size() - 1);

        std::string result;
        result.reserve(length);
        for (int i = 0; i < length; ++i) {
            result.push_back(ALLOWED_CHARS[charDist(rng)]);
        }
        return result;
    }

public:
    // Constructor for the generator (seed can be fixed if needed for reproducibility)
    StringGenerator(unsigned int seed = std::random_device{}()) {
        rng.seed(seed);
    }

    // Generate an array of random strings of the specified size
    std::vector<std::string> generateRandomArray(size_t size) {
        std::vector<std::string> arr;
        arr.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            arr.push_back(generateSingleString());
        }
        return arr;
    }

    // Generate an array sorted in reverse lexicographical order
    std::vector<std::string> generateReverseSortedArray(size_t size) {
        auto arr = generateRandomArray(size);
        std::sort(arr.begin(), arr.end());
        std::reverse(arr.begin(), arr.end());
        return arr;
    }

    // Generate a nearly sorted array by performing a small number of random swaps
    std::vector<std::string> generateNearlySortedArray(size_t size, int swaps = 50) {
        auto arr = generateRandomArray(size);
        std::sort(arr.begin(), arr.end());
        if (arr.size() < 2) return arr;

        std::uniform_int_distribution<size_t> dist(0, arr.size() - 1);
        for (int i = 0; i < swaps; ++i) {
            size_t i1 = dist(rng), i2 = dist(rng);
            std::swap(arr[i1], arr[i2]);
        }
        return arr;
    }

    // Generate an array of random strings with a common prefix
    std::vector<std::string> generatePrefixArray(size_t size, const std::string& prefix = "ABC") {
        std::vector<std::string> arr;
        arr.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            arr.push_back(prefix + generateSingleString());
        }
        return arr;
    }

    // Extract a sub-array of the specified target size
    std::vector<std::string> getSubArray(const std::vector<std::string>& fullArray, size_t targetSize) {
        if (targetSize >= fullArray.size()) return fullArray;
        return std::vector<std::string>(fullArray.begin(), fullArray.begin() + targetSize);
    }
};