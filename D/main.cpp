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

std::string GetReverseString(const std::string& str) {
    auto string_copy = str;
    std::reverse(string_copy.begin(), string_copy.end());
    return string_copy;
}

uint32_t GetNumberOfDifferentSubstring(const std::string& str) {
    std::string string_copy;
    uint32_t number_of_different_substring = 0;
    for (uint32_t symbol_position = 0; symbol_position < str.length(); ++symbol_position) {
        string_copy += str[symbol_position];
        auto string_reverse_copy = GetReverseString(string_copy);
        auto prefix = PrefixFunction(string_reverse_copy);
        uint32_t max_prefix = 0;
        for (uint32_t iteration = 0; iteration < prefix.size(); ++iteration) {
            max_prefix = std::max(max_prefix, prefix[iteration]);
        }
        number_of_different_substring += (string_copy.length() - max_prefix);
    }

    return number_of_different_substring;
}

int main() {
    std::string str;
    std::cin >> str;

    std::cout << GetNumberOfDifferentSubstring(str) << std::endl;
    return 0;
}
