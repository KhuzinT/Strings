#include <iostream>
#include <algorithm>
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

std::string Encryption(const std::vector<std::string>& dictionary) {
    std::string result = dictionary[0];
    for (uint32_t iteration = 1; iteration < dictionary.size(); ++iteration) {
        auto str = dictionary[iteration];

        uint32_t shift = 0;
        if (str.size() < result.size()) {
            shift = result.size() - str.size();
        }

        auto new_str = result.substr(shift);
        std::string str_to_prefix = str + "#" + new_str;
        auto prefix = PrefixFunction(str_to_prefix);

        uint32_t prefix_back = prefix.size() - 1;
        result += str.substr(prefix[prefix_back], str.size() - prefix[prefix_back]);
    }

    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    uint32_t word_count = 0;
    std::cin >> word_count;

    std::vector<std::string> dictionary(word_count);
    for (uint32_t iteration = 0; iteration < word_count; ++iteration) {
        std::cin >> dictionary[iteration];
    }

    std::cout << Encryption(dictionary) << std::endl;
    return 0;
}