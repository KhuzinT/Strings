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

uint32_t FindNumberOfCyclicPermutationsOfStringInText(const std::string& text, const std::string& str) {
    std::string string_to_prefix = text + "#" + str + str;
    string_to_prefix.resize(string_to_prefix.size());

    uint32_t number_of_cyclic_permutation_of_string_in_text = 0;

    auto prefix = PrefixFunction(string_to_prefix);
    for (uint32_t iteration = 0; iteration < prefix.size(); ++iteration) {
        if (iteration != prefix.size() - 1 && prefix[iteration] >= str.size()) {
            ++number_of_cyclic_permutation_of_string_in_text;
        }
    }

    return number_of_cyclic_permutation_of_string_in_text;
}

int main() {
    std::string str;
    std::cin >> str;

    std::string text;
    std::cin >> text;

    std::cout << FindNumberOfCyclicPermutationsOfStringInText(text, str) << std::endl;

    return 0;
}
