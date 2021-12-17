#include <iostream>
#include <string>
#include <vector>

std::vector<uint32_t> PrefixFunction(const std::string& str) {
    std::vector<uint32_t> prefix(str.length(), 0);
    for (uint32_t position = 1; position < str.length(); ++position) {
        uint32_t current_len = prefix[position - 1];
        while (current_len > 0) {
            if (str[current_len] == str[position]) {
                break;
            }
            current_len = prefix[current_len - 1];
        }
        if (str[position] == str[current_len]) {
            ++current_len;
        }
        prefix[position] = current_len;
    }
    return prefix;
}

int main() {
    std::string str;
    std::cin >> str;

    auto prefix = PrefixFunction(str);
    for (auto& elem : prefix) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
    return 0;
}
