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

std::vector<std::string> SplitOneWordIntoPrefixesOfAnotherWord(const std::string& word_of_prefix,
                                                               const std::string& split_word) {
    std::string string_to_prefix = word_of_prefix + "#" + split_word;
    auto prefix = PrefixFunction(string_to_prefix);

    for (uint32_t iteration = 0; iteration < prefix.size(); ++iteration) {
        if (iteration > word_of_prefix.size()) {
            if (prefix[iteration] == 0) {
                return {};
            }
        }
        if (iteration < split_word.size()) {
            if (word_of_prefix.find(split_word[iteration]) == std::string::npos) {
                return {};
            }
        }
    }

    std::vector<std::string> prefixes;

    for (uint32_t position = prefix.size() - 1; position > word_of_prefix.length();) {
        auto new_prefix = string_to_prefix.substr(position - prefix[position] + 1, prefix[position]);
        prefixes.push_back(new_prefix);
        position -= prefix[position];
    }

    std::reverse(prefixes.begin(), prefixes.end());
    return prefixes;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string first_word;
    std::cin >> first_word;

    std::string last_word;
    std::cin >> last_word;

    auto list_of_prefixes = SplitOneWordIntoPrefixesOfAnotherWord(first_word, last_word);
    if (list_of_prefixes.empty()) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "No" << std::endl;
        for (auto& str : list_of_prefixes) {
            std::cout << str << ' ';
        }
        std::cout << std::endl;
    }
}