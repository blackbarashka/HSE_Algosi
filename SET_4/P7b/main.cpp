#include <iostream>
size_t hash(const std::string &str) {
    return str[0] & 7;
}
int main() {
    std::cout << hash("a");
}