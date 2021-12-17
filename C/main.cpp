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
    char separator = '#';
    std::string str_to_find = str + separator + text;
    auto prefix = PrefixFunction(str_to_find);
    for (uint32_t iteration = 0; iteration < prefix.size(); ++iteration) {
        if (prefix[iteration] == str.length() && iteration >= 2 * str.length()) {
            number_of_entry.push_back(iteration - 2 * str.length());
        }
    }
    return number_of_entry;
}

bool IsStringInText(const std::string& text, const std::string& str) {
    auto number_of_entry = FindEntryOfStringInText(text, str);
    return !number_of_entry.empty();
}

enum Cases { THE_ZERO_IN_UNKNOWN_PLACE = 0, THE_ONE_IN_UNKNOWN_PLACE = 1, UNKNOWN = 2 };

uint32_t WhatStandsInUnknownPlace(const std::string& str_before, const std::string& str_after) {
    std::string str_before_with_zero = str_before + "0" + str_before;
    std::string str_before_with_one = str_before + "1" + str_before;

    std::string str_after_with_zero = str_after + "0";
    std::string str_after_with_one = str_after + "1";

    if (IsStringInText(str_before_with_zero, str_after_with_zero) ||
        IsStringInText(str_before_with_one, str_after_with_zero)) {
        if (!IsStringInText(str_before_with_zero, str_after_with_one) &&
            !IsStringInText(str_before_with_one, str_after_with_one)) {
            return THE_ZERO_IN_UNKNOWN_PLACE;
        }
    }

    if (IsStringInText(str_before_with_zero, str_after_with_one) ||
        IsStringInText(str_before_with_one, str_after_with_one)) {
        if (!IsStringInText(str_before_with_zero, str_after_with_zero) &&
            !IsStringInText(str_before_with_one, str_after_with_zero)) {
            return THE_ONE_IN_UNKNOWN_PLACE;
        }
    }

    return UNKNOWN;
}

int main() {
    uint32_t string_len = 0;
    std::cin >> string_len;

    std::string string_before;
    std::cin >> string_before;

    std::string string_after;
    std::cin >> string_after;

    uint32_t what_stands = WhatStandsInUnknownPlace(string_before, string_after);
    if (what_stands == THE_ZERO_IN_UNKNOWN_PLACE) {
        std::cout << "No" << std::endl;
    } else if (what_stands == THE_ONE_IN_UNKNOWN_PLACE) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "Random" << std::endl;
    }

    return 0;
}
