#include "bloom_filter.h"
//Musaev Umakhan.
BloomFilter::BloomFilter(size_t numHashes, size_t numBits)
        : _hashes(numHashes), _bits(numBits), _filter(numBits, false) {
}


void BloomFilter::add(const std::string& key) {
    for (size_t i = 0; i < _hashes; ++i) {
        size_t hash = _get_hash(i, key);
        _filter[hash] = true;
    }
    _raw_set.insert(key);
}

bool BloomFilter::verify(const std::string& key) {
    cnt++;

    for (size_t i = 0; i < _hashes; ++i) {
        size_t hash = _get_hash(i, key);
        if (!_filter[hash]) {
            return false;
        }
    }
    if (_raw_set.find(key) == _raw_set.end()) {
        // The key is not in the actual set; increment false positive count.
        false_positive_cnt++;
    }
    return true;
}

double BloomFilter::getFPRate() const {
    if (cnt == 0) {
        return 0.0;
    }
    return static_cast<double>(false_positive_cnt) / cnt;
}

size_t BloomFilter::numberOfHashFunctions() const {
    return _hashes;
}

size_t BloomFilter::numberOfBits() const {
    // Return the size of the bit array.
    return _bits;
}

inline size_t BloomFilter::_get_hash(int i, const std::string& key) const {
    std::string salted_key = key;
    if (i > 0) {
        salted_key += std::to_string(i);
    }
    size_t hash = hasher(salted_key) % _bits;
    return hash;
}

int main() {
    BloomFilter bf(4, 1000); // 4 hash functions, 1000 bits
    bf.add("hello");
    bf.add("world");

    if (bf.verify("hello")) {
        std::cout << "'hello' might be in the set.\n";
    } else {
        std::cout << "'hello' is definitely not in the set.\n";
    }

    if (bf.verify("test")) {
        std::cout << "'test' might be in the set.\n";
    } else {
        std::cout << "'test' is definitely not in the set.\n";
    }

    std::cout << "False positive rate: " << bf.getFPRate() << std::endl;

    return 0;
}