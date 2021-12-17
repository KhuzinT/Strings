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

std::vector<uint32_t> FindEntryOfStringInText(const std::string& text, const std::string& str) {
    std::vector<uint32_t> number_of_entry;
    char separator = '0';
    std::string str_to_find = str + separator + text;
    auto prefix = PrefixFunction(str_to_find);
    for (uint32_t iteration = 0; iteration < prefix.size(); ++iteration) {
        if (prefix[iteration] == str.length() && iteration >= 2 * str.length()) {
            number_of_entry.push_back(iteration - 2 * str.length());
        }
    }
    return number_of_entry;
}

int main() {
    std::string text;
    std::string str;

    std::cin >> text;
    std::cin >> str;

    auto number_of_entry = FindEntryOfStringInText(text, str);
    for (auto& elem : number_of_entry) {
        std::cout << elem << std::endl;
    }
    return 0;
}
